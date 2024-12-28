/*
Copyright (c) 2021-2024 Nicolas Beddows <nicolas.beddows@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <memory>
#ifdef ENABLE_NLOHMANN_JSON
#include <nlohmann/json.hpp>
#else
#include <ArduinoJson.h>
#endif
#ifdef ENABLE_MEEN_RP2040
#include <pico/stdlib.h>
#endif
#include <unity/unity.h>

#include "meen/Error.h"
#include "meen/MachineFactory.h"
#include "test_controllers/MemoryController.h"
#include "test_controllers/TestIoController.h"
#include "test_controllers/CpmIoController.h"

namespace meen::tests
{
    static std::shared_ptr<IController> cpmIoController;
    static std::shared_ptr<MemoryController> memoryController;
    static std::shared_ptr<IController> testIoController;
    static std::unique_ptr<IMachine> machine;
    static std::string programsDir;

    static void Run(bool runAsync)
    {
        std::error_code err;

        if (runAsync == true)
        {
            err = machine->SetOptions(R"({"runAsync":true})");
            TEST_ASSERT_FALSE(err);
        }

        // Run a program that should take a second to complete
        // (in actual fact it's 2000047 ticks, 47 ticks over a second.
        // We need to be as close a possible to 2000000 ticks without
        // going under so the cpu sleeps at the end
        // of the program so it maintains sync. It's never going to
        // be perfect, but its close enough for testing purposes).
        err = memoryController->Load((programsDir + "nopStart.bin").c_str(), 0x04);
        TEST_ASSERT_FALSE(err);
        
        err = memoryController->Load((programsDir + "nopEnd.bin").c_str(), 0xC353);
        TEST_ASSERT_FALSE(err);

        // 25 millisecond resolution, service interrupts every 8.25 milliseconds
        err = machine->SetOptions(R"({"clockResolution":25000000, "isrFreq":0.25})");
        TEST_ASSERT_FALSE(err);

        int64_t nanos = 0;

        err = machine->Run(0x04);
        TEST_ASSERT_FALSE(err);

// Use std::expected monadics if they are supported
#if ((defined __GNUC__ && __GNUC__ >= 13) || (defined _MSC_VER && _MSC_VER >= 1706))
        nanos += machine->WaitForCompletion().or_else([](std::error_code ec)
        {
            // We want to force a failure here, ec should be non zero
            TEST_ASSERT_FALSE(ec);
            // The machine didn't run, return a run time of zero
            return std::expected<uint64_t, std::error_code>(0);
        }).value();
#else
        auto ex = machine->WaitForCompletion();
        TEST_ASSERT_TRUE(ex);

        nanos += ex.value();
#endif
        auto error = nanos - 1000000000;
        // Allow an average 500 micros of over sleep error
        TEST_ASSERT_TRUE(error >= 0 && error <= 500000);
    }

    static void Load(bool runAsync)
    {
        std::vector<std::string> saveStates;
        auto err = machine->OnSave([&](const char* json) { saveStates.emplace_back(json); });

        if(err.value() == errc::json_config)
        {
            // not implemented, skip the test
            TEST_IGNORE_MESSAGE("Machine::OnSave not supported");
        }

        // 0 - mid program save state, 1 and 2 - end of program save states
        err = machine->OnLoad([&] { return saveStates[0].c_str(); });

        if(err.value() == errc::json_config)
        {
            // not implemented, skip the test
            TEST_IGNORE_MESSAGE("Machine::OnLoad not supported");
        }

        if (runAsync == true)
        {
            err = machine->SetOptions(R"({"runAsync":true,"loadAsync":false,"saveAsync":true})");

            if(err.value() == errc::json_config)
            {
                // not implemented, skip the test
                TEST_IGNORE_MESSAGE("runAsync:true not supported");
            }

            TEST_ASSERT_FALSE(err);
        }

        auto cpm = static_pointer_cast<CpmIoController>(cpmIoController);
        // Trigger a save when the 3000th cycle has executed.
        cpm->SaveStateOn(3000);
        // Call the out instruction
        memoryController->Write(0x00FE, 0xD3);
        // The data to write to the controller that will trigger the ISR::Load interrupt
        memoryController->Write(0x00FF, 0xFD);

        err = memoryController->Load((programsDir + "/TST8080.COM").c_str(), 0x100);
        TEST_ASSERT_FALSE(err);
        // Set the rom/ram offsets for tst8080, note that tst8080 uses 256 bytes of stack space
        // located at the end of the program so this will make up the ram size since the program
        // never writes beyond this.
        err = machine->SetOptions(R"({"rom":{"file":[{"offset":0,"size":1727}]},"ram":{"block":[{"offset":1727,"size":256}]}})");
        TEST_ASSERT_FALSE(err);
        machine->SetIoController(cpmIoController);

        err = machine->Run(0x0100);
        TEST_ASSERT_FALSE(err);

        auto ex = machine->WaitForCompletion();
        TEST_ASSERT_TRUE(ex);

        TEST_ASSERT_EQUAL_INT(74, cpm->Message().find("CPU IS OPERATIONAL"));

        // Disable triggering a save from this controller so the other cpm tests will pass.
        // Needs to be done before the next Run call so the async version of this test won't
        // trigger a spurious ISR::Save interurpt if the ISR::Load interrupt takes too long
        // to process
        cpm->SaveStateOn(-1);

        // run it again, but this time trigger the load interrupt
        err = machine->Run(0x00FE);
        TEST_ASSERT_FALSE(err);

        ex = machine->WaitForCompletion();
        TEST_ASSERT_TRUE(ex);

        // Since we are not saving/loading the io state the contents of the message buffer can
        // be in one of two states depending on how long the OnLoad initiation handler took to complete.
        auto pos = cpm->Message().find("CPU IS OPERATIONAL");

        // Currently we are not saving the state of the io (do we need to?????)
        // This can cause variable output as discussed below
        if (runAsync == true)
        {
            // If the OnLoad initiation handler was quick to complete (sub 150 ticks) the preamble message would
            // not have been written to the message string and the success message should be found at pos 3, otherwise
            // the preamble message was written and it should be found at pos 74
            TEST_ASSERT_TRUE(3 == pos || 74 == pos);
        }
        else
        {
            // Since we loaded mid program the message from the tests won't contain the premable
            // (since we are not saving/loading the io state), just the result,
            // hence we should find the success message earlier in the message string.
            TEST_ASSERT_EQUAL_INT(3, pos);
        }

        // When we are in the middle of a save when another save is requested it will be dropped.
        // This may or may not happen depending on how fast the first save takes to complete.
        TEST_ASSERT_TRUE(saveStates.size() == 3 || saveStates.size() == 2);
        TEST_ASSERT_EQUAL_STRING(R"({"cpu":{"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":19,"b":19,"c":0,"d":19,"e":0,"h":19,"l":0,"s":86},"pc":1236,"sp":1981},"memory":{"uuid":"zRjYZ92/TaqtWroc666wMQ==","rom":"JXg8/M+WvmCGVMmH7xr/0g==","ram":{"encoder":"base64","compressor":"zlib","size":256,"bytes":"eJwLZRhJQJqZn5mZ+TvTa6b7TJeZjjIxMAAAfY0E7w=="}}})", saveStates[0].c_str());
        TEST_ASSERT_EQUAL_STRING(R"({"cpu":{"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":170,"b":170,"c":9,"d":170,"e":170,"h":170,"l":170,"s":86},"pc":2,"sp":1981},"memory":{"uuid":"zRjYZ92/TaqtWroc666wMQ==","rom":"JXg8/M+WvmCGVMmH7xr/0g==","ram":{"encoder":"base64","compressor":"zlib","size":256,"bytes":"eJw7w2ZczrCXnWFkAGlmfmZm5u9MYauCGFet2sXGwAAAYNgG1w=="}}})", saveStates[1].c_str());

        if (saveStates.size() == 3)
        {
            TEST_ASSERT_EQUAL_STRING(R"({"cpu":{"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":170,"b":170,"c":9,"d":170,"e":170,"h":170,"l":170,"s":86},"pc":2,"sp":1981},"memory":{"uuid":"zRjYZ92/TaqtWroc666wMQ==","rom":"JXg8/M+WvmCGVMmH7xr/0g==","ram":{"encoder":"base64","compressor":"zlib","size":256,"bytes":"eJw7w2ZczrCXnWFkAGlmfmZm5u9MYauCGFet2sXGwAAAYNgG1w=="}}})", saveStates[2].c_str());
        }
    }

    static void LoadAndRun(const char* name, const char* expected)
    {
        machine->OnSave([expected](const char* actual)
        {
            std::string actualStr;
            std::string expectedStr;
#ifdef ENABLE_NLOHMANN_JSON
            auto actualJson = nlohmann::json::parse(actual, nullptr, false);
            TEST_ASSERT_FALSE(actualJson.is_discarded());
            auto expectedJson = nlohmann::json::parse(expected, nullptr, false);
            TEST_ASSERT_FALSE(expectedJson.is_discarded());
            expectedStr = expectedJson.dump();
            actualStr = actualJson["cpu"].dump();
#else
            JsonDocument actualJson;
            JsonDocument expectedJson;
            auto err = deserializeJson(actualJson, actual);
            TEST_ASSERT_FALSE(err);
            err = deserializeJson(expectedJson, expected);
            TEST_ASSERT_FALSE(err);
            serializeJson(actualJson["cpu"], actualStr);
            serializeJson(expectedJson, expectedStr);
#endif
            TEST_ASSERT_EQUAL_STRING(expectedStr.c_str(), actualStr.c_str());
        });

        auto dir = programsDir + name;

        auto err = memoryController->Load(dir.c_str(), 0x100);
        TEST_ASSERT_FALSE(err);

        err = machine->Run(0x100);
        TEST_ASSERT_FALSE(err);
    }

    static void suiteSetUp()
    {
        // Note that the tests don't require a json string to be set as it just uses the default values,
        // it is used here for demonstation purposes only
        machine = Make8080Machine();
        memoryController = std::make_shared<MemoryController>();
        cpmIoController = std::make_shared<CpmIoController>(static_pointer_cast<IController>(memoryController));
        testIoController = std::make_shared<TestIoController>();

        // Use the default directory if it has not been set by the user
        if(programsDir.empty() == true)
        {
             programsDir = PROGRAMS_DIR;
        }
    }

    static int suiteTearDown(int numFailures)
    {
        return numFailures;
    }

    static void test_SetNullptrMemoryController()
    {
        auto errc = machine->SetMemoryController(nullptr);
	    TEST_ASSERT_TRUE(errc);
        TEST_ASSERT_EQUAL_STRING("An argument supplied to the method is invalid", errc.message().c_str());
    }

    static void test_SetNullptrIoController()
    {
        //cppcheck-suppress unknownMacro
        auto errc = machine->SetIoController(nullptr);
        TEST_ASSERT_TRUE(errc);
        TEST_ASSERT_EQUAL_STRING("An argument supplied to the method is invalid", errc.message().c_str());
    }

    static void test_NegativeISRFrequency()
    {
        //cppcheck-suppress unknownMacro
        auto errc = machine->SetOptions(R"({"isrFreq":-1.0})");
        TEST_ASSERT_TRUE(errc);
    }

    static void test_MethodsErrorAfterRunCalled()
    {
        //cppcheck-suppress unknownMacro
        // Set the resolution so the Run method takes about 1 second to complete therefore allowing subsequent IMachine method calls to return errors
        auto err = machine->SetOptions(R"({"clockResolution":25000000,"runAsync":true,"isrFreq":0.25})"); // must be async so the Run method returns immediately
        TEST_ASSERT_FALSE(err);

        err = memoryController->Load((programsDir + "nopStart.bin").c_str(), 0x04);
        TEST_ASSERT_FALSE(err);

        err = memoryController->Load((programsDir + "nopEnd.bin").c_str(), 0xC353);
        TEST_ASSERT_FALSE(err);

        // We aren't interested in saving, clear the onSave callback
        err = machine->OnSave(nullptr);
        TEST_ASSERT_TRUE(err.value() == errc::no_error || err.value() == errc::json_config);
 
        err = machine->Run(0x04);
        TEST_ASSERT_FALSE(err);

        // All these methods should return errors
        //machine->Run(0x100);
        err = machine->SetOptions(R"({"isrFreq":1})");
        TEST_ASSERT_TRUE(err);
        err = machine->SetMemoryController(memoryController);
        TEST_ASSERT_TRUE(err);
        err = machine->SetIoController(testIoController);
        TEST_ASSERT_TRUE(err);
        err = machine->OnLoad([]{ return ""; });
        TEST_ASSERT_TRUE(err);
        err = machine->OnSave([](const char*){});
        TEST_ASSERT_TRUE(err);

        // Since we are running async we need to wait for completion
        auto ex = machine->WaitForCompletion();
        TEST_ASSERT_TRUE(ex);

        // We are now no longer running, all these methods should not return errors
        err = machine->SetOptions(R"({"isrFreq":1})");
        TEST_ASSERT_FALSE(err);
        err = machine->SetMemoryController(memoryController);
        TEST_ASSERT_FALSE(err);
        err = machine->SetIoController(testIoController);
        TEST_ASSERT_FALSE(err);
        err = machine->OnLoad([]{ return ""; });
        TEST_ASSERT_TRUE(err.value() == errc::no_error || err.value() == errc::json_config);
    }

    static void test_RunTimed()
    {
        Run(false);
    }

    static void test_RunTimedAsync()
    {
        Run(true);
    }

    static void test_OnLoad()
    {
        for (int i = 0; i < 50; i++)
        {
            Load(false);
        }
    }

    static void test_OnLoadAsync()
    {
        for (int i = 0; i < 50; i++)
        {
            Load(true);
        }
    }

    static void test_Tst8080()
    {
        // use the cpm io controller for cpm based tests
        auto err = machine->SetIoController(cpmIoController);
        TEST_ASSERT_FALSE(err);
        err = machine->SetOptions(R"({"isrFreq":0.02})");
        TEST_ASSERT_FALSE(err);
        LoadAndRun("TST8080.COM", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":170,"b":170,"c":9,"d":170,"e":170,"h":170,"l":170,"s":86},"pc":2,"sp":1981})");
        TEST_ASSERT_EQUAL_INT(74, static_pointer_cast<CpmIoController>(cpmIoController)->Message().find("CPU IS OPERATIONAL"));
    }

    static void test_8080Pre()
    {
        // use the cpm io controller for cpm based tests
        auto err = machine->SetIoController(cpmIoController);
        TEST_ASSERT_FALSE(err);
        err = machine->SetOptions(R"({"isrFreq":0.02})");
        TEST_ASSERT_FALSE(err);
        LoadAndRun("8080PRE.COM", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":9,"d":3,"e":50,"h":1,"l":0,"s":86},"pc":2,"sp":1280})");
        TEST_ASSERT_EQUAL_INT(0, static_pointer_cast<CpmIoController>(cpmIoController)->Message().find("8080 Preliminary tests complete"));
    }

    static void test_CpuTest()
    {
        // use the cpm io controller for cpm based tests
        auto err = machine->SetIoController(cpmIoController);
        TEST_ASSERT_FALSE(err);
        err = machine->SetOptions(R"({"isrFreq":0.02})");
        TEST_ASSERT_FALSE(err);
        LoadAndRun("CPUTEST.COM", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":247,"d":4,"e":23,"h":0,"l":0,"s":70},"pc":2,"sp":12283})");
        TEST_ASSERT_EQUAL_INT(168, static_pointer_cast<CpmIoController>(cpmIoController)->Message().find("CPU TESTS OK"));
    }

    static void test_8080Exm()
    {
        // use the cpm io controller for cpm based tests
        auto err = machine->SetIoController(cpmIoController);
        TEST_ASSERT_FALSE(err);
        err = machine->SetOptions(R"({"isrFreq":0.02})");
        TEST_ASSERT_FALSE(err);
        LoadAndRun("8080EXM.COM", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":10,"c":9,"d":14,"e":30,"h":1,"l":109,"s":70},"pc":2,"sp":54137})");
        TEST_ASSERT_EQUAL_INT(static_pointer_cast<CpmIoController>(cpmIoController)->Message().find("ERROR"), std::string::npos);
    }
} // namespace meen::tests

void setUp()
{
    meen::tests::memoryController->Clear();
    //CP/M Warm Boot is at memory address 0x00, this will be
    //emulated with the exitTest subroutine.
    auto err = meen::tests::memoryController->Load((meen::tests::programsDir + "/exitTest.bin").c_str(), 0x00);
    TEST_ASSERT_FALSE(err);

    //CP/M BDOS print message system call is at memory address 0x05,
    //this will be emulated with the bdosMsg subroutine.
    err = meen::tests::memoryController->Load((meen::tests::programsDir + "/bdosMsg.bin").c_str(), 0x05);
    TEST_ASSERT_FALSE(err);
    
    meen::tests::machine->SetMemoryController(meen::tests::memoryController);
    meen::tests::machine->SetIoController(meen::tests::testIoController);
    
    // Set default options
    err = meen::tests::machine->SetOptions(nullptr);
    TEST_ASSERT_FALSE(err);
}

void tearDown()
{

}

/*
#include <malloc.h>

uint32_t GetTotalHeap()
{
    extern char __StackLimit, __bss_end__;
    return &__StackLimit - &__bss_end__;
}
*/

int main(int argc, char** argv)
{
    int err = 0;

    if (argc > 1)
    {
        meen::tests::programsDir = argv[1];
    }

#ifdef ENABLE_MEEN_RP2040
    stdio_init_all();

//    set_sys_clock_pll(150, 5, 2);
//    set_sys_clock_khz(250000, true);

    while(true)
#endif
    {
        meen::tests::suiteSetUp();
        UNITY_BEGIN();
        RUN_TEST(meen::tests::test_SetNullptrIoController);
        RUN_TEST(meen::tests::test_SetNullptrMemoryController);
        RUN_TEST(meen::tests::test_NegativeISRFrequency);
        RUN_TEST(meen::tests::test_OnLoad);
        RUN_TEST(meen::tests::test_OnLoadAsync);
        RUN_TEST(meen::tests::test_Tst8080);
        RUN_TEST(meen::tests::test_8080Pre);
        //RUN_TEST(meen::tests::test_CpuTest);
        //RUN_TEST(meen::tests::test_8080Exm);
        RUN_TEST(meen::tests::test_MethodsErrorAfterRunCalled);
        RUN_TEST(meen::tests::test_RunTimed);
        RUN_TEST(meen::tests::test_RunTimedAsync);
        err = meen::tests::suiteTearDown(UNITY_END());

        //struct mallinfo m = mallinfo();
        //printf("FREE HEAP: %d\n", GetTotalHeap() - m.uordblks);
    }

    return err;
}