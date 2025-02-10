/*
Copyright (c) 2021-2025 Nicolas Beddows <nicolas.beddows@gmail.com>

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
#include <stdarg.h>
#include <unity/unity.h>

#include "meen/Error.h"
#include "meen/MachineFactory.h"
#include "test_controllers/MemoryController.h"
#include "test_controllers/TestIoController.h"
#include "test_controllers/CpmIoController.h"

static std::unique_ptr<meen::IMachine> machine;
static std::string programsDir;
// A base64 encoded code fragment that is loaded at address 0x0000 (for test suite compatibility) which saves the current machine state, powers off the machine, then halts the cpu.
static const char* saveAndExit = "base64://0/7T/3Y";
// A base64 encoded code fragment that initialises the 'a' register to 10 and is loaded at address 0x0005. Used in conjunction with nopEnd to run the timing test
static const char* nopStart = "base64://Pgo=";
// A base64 encoded code fragment that decrements the 'a' register by 1, jumps to address 0x0005 if 'a' is non-zero else address 0x0000.
static const char* nopEnd = "base64://PcIHAMMAAA";
// A base64 encoded code fragment that emulates cp/m bdos function 4 - raw console output.  
static const char* bdosMsg = "base64://9XnTAP4CyhEAetMBe9MC8ck=";

void setUp()
{
    auto controller = machine->DetachIoController();
    TEST_ASSERT_TRUE(controller);
    // Write to the 'load device', the value doesn't matter (use 0)
    controller.value()->Write(0xFD, 0, nullptr);
    auto err = machine->AttachIoController(std::move(controller.value()));
    TEST_ASSERT_FALSE(err);

    // Set default options
    err = machine->SetOptions(nullptr);
    TEST_ASSERT_FALSE(err);
}

void tearDown()
{
    // Unused
}

namespace meen::tests
{
    static IControllerPtr cpmIoController;

    static void suiteSetUp()
    {
        machine = Make8080Machine();
        TEST_ASSERT_TRUE(machine);

        cpmIoController = IControllerPtr(new CpmIoController());

        // Use the default directory if it has not been set by the user
        if (programsDir.empty() == true)
        {
            programsDir = PROGRAMS_DIR;
        }

        auto err = machine->AttachMemoryController(IControllerPtr(new MemoryController()));
        TEST_ASSERT_FALSE(err);
        err = machine->AttachIoController(IControllerPtr(new TestIoController()));
        TEST_ASSERT_FALSE(err);
    }

    static int suiteTearDown(int numFailures)
    {
        return numFailures;
    }

    // todo: the returned 'json' needs to be prefixed with a protcol so it can be read from file:// for example
    static errc LoadProgram(char* json, int* jsonLen, const char* fmt, ...)
    {
        va_list args;
        va_start(args, fmt);

        if (json == nullptr)
        {
            *jsonLen = vsnprintf(nullptr, 0, fmt, args);
            return *jsonLen == -1 ? errc::invalid_argument : errc::no_error;
        }
        else
        {
            return vsnprintf(json, *jsonLen, fmt, args) != *jsonLen - 1 ? errc::invalid_argument : errc::no_error;
        }
    }

    static void Run(bool runAsync)
    {
        int64_t nanos = 0;
        std::error_code err;

        if (runAsync == true)
        {
            err = machine->SetOptions(R"({"runAsync":true})");
            TEST_ASSERT_FALSE(err);
        }

        err = machine->OnLoad([progDir = programsDir.c_str()](char* json, int* jsonLen)
        {
            return LoadProgram(json, jsonLen, R"({"cpu":{"pc":5},"memory":{"rom":{"block":[{"bytes":"%s","offset":0},{"bytes":"%s","offset":5},{"bytes":"%s","offset":50004}]}}})", saveAndExit, nopStart, nopEnd);
        });
        TEST_ASSERT_FALSE(err);

        // 25 millisecond resolution, service interrupts every 6.25 milliseconds
        err = machine->SetOptions(R"({"clockResolution":25000000, "isrFreq":0.25})");
        TEST_ASSERT_FALSE(err);

        err = machine->Run();
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
        int loadIndex = 0;
        std::vector<std::string> saveStates;
        auto err = machine->OnSave([&](const char* json)
        {
            saveStates.emplace_back(json);
            return errc::no_error;
        });

        if(err.value() == errc::not_implemented)
        {
            // not implemented, skip the test
            TEST_IGNORE_MESSAGE("Machine::OnSave not supported");
        }

        // 0 - mid program save state, 1 and 2 - end of program save states
        err = machine->OnLoad([&saveStates, &loadIndex, progDir = programsDir.c_str()](char* json, int* jsonLen)
        {
            auto err = errc::no_error;

            switch (loadIndex)
            {
                // Tst8080 test file includes a small amount of stack space
                // located at the end of the program. This ram needs to
                // be subtracted from the total size of the file,
                // hence an explicit setting of the test file size.
            case 0:
                err = LoadProgram(json, jsonLen, R"({"cpu":{"pc":256},"memory":{"rom":{"block":[{"bytes":"%s","offset":0},{"bytes":"%s","offset":5},{"bytes":"file://%s/TST8080.COM","offset":256,"size":1471}]}}})", saveAndExit, bdosMsg, progDir);
                break;
            case 1:
                // 0 - mid program save state, 1 and 2 - end of program save states
                err = LoadProgram(json, jsonLen, saveStates[0].c_str());
                break;
            default:
                err = errc::invalid_argument;
                break;
            }

            loadIndex += json != nullptr;

            return err;
        });

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

        // Write to the 'load device', the value doesn't matter (use 0)
        cpmIoController->Write(0xFD, 0, nullptr);

        auto cpm = static_cast<CpmIoController*>(cpmIoController.get());
        // Trigger a save when the 3000th cycle has executed.
        cpm->SaveStateOn(3000);

        auto controller = machine->DetachIoController();
        TEST_ASSERT_TRUE(controller);

        err = machine->AttachIoController(std::move(cpmIoController));
        TEST_ASSERT_FALSE(err);

        err = machine->Run();
        TEST_ASSERT_FALSE(err);

        auto expected = machine->WaitForCompletion();
        TEST_ASSERT_TRUE(expected);

        cpmIoController = std::move(machine->DetachIoController().value());
        TEST_ASSERT_TRUE(cpmIoController);

        // Write to the 'load device', the value doesn't matter (use 0)
        cpmIoController->Write(0xFD, 0, nullptr);

        cpm = static_cast<CpmIoController*>(cpmIoController.get());

        TEST_ASSERT_EQUAL_INT(74, cpm->Message().find("CPU IS OPERATIONAL"));

        // Disable triggering a save from this controller so the other cpm tests will pass.
        // Needs to be done before the next Run call so the async version of this test won't
        // trigger a spurious ISR::Save interurpt if the ISR::Load interrupt takes too long
        // to process
        cpm->SaveStateOn(-1);

        err = machine->AttachIoController(std::move(cpmIoController));
        TEST_ASSERT_FALSE(err);

        // run it again, but this time trigger the load interrupt
        err = machine->Run();
        TEST_ASSERT_FALSE(err);

        expected = machine->WaitForCompletion();
        TEST_ASSERT_TRUE(expected);

        cpmIoController = std::move(machine->DetachIoController().value());
        TEST_ASSERT_TRUE(cpmIoController);

        cpm = static_cast<CpmIoController*>(cpmIoController.get());
        TEST_ASSERT_TRUE(cpm);

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
        TEST_ASSERT_EQUAL_STRING(R"({"cpu":{"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":19,"b":19,"c":0,"d":19,"e":0,"h":19,"l":0,"s":86},"pc":1236,"sp":1981},"memory":{"uuid":"base64://zRjYZ92/TaqtWroc666wMQ==","rom":{"bytes":"base64://md5://BVt1f9Z97W/m34J/iH68cQ=="},"ram":{"size":64042,"bytes":"base64://zlib://eJztzlENgDAQBbDlnQAETBeSpwABCEDAfnHBktEqaGt/ca4OfKrXUVUzT+5cGVn9AQAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAGBXL4n+BO8="}}})", saveStates[0].c_str());
        TEST_ASSERT_EQUAL_STRING(R"({"cpu":{"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":170,"b":170,"c":9,"d":170,"e":170,"h":170,"l":170,"s":86},"pc":2,"sp":1981},"memory":{"uuid":"base64://zRjYZ92/TaqtWroc666wMQ==","rom":{"bytes":"base64://md5://BVt1f9Z97W/m34J/iH68cQ=="},"ram":{"size":64042,"bytes":"base64://zlib://eJztzkENgDAQALDBJeOJAGTghAdW8HQSSHAwP3xxwRJoFbSUv2h1Pco19W68ZIk5Iu5xz23IPGvvDwAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABf9QDDAAbX"}}})", saveStates[1].c_str());

        if (saveStates.size() == 3)
        {
            TEST_ASSERT_EQUAL_STRING(R"({"cpu":{"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":170,"b":170,"c":9,"d":170,"e":170,"h":170,"l":170,"s":86},"pc":2,"sp":1981},"memory":{"uuid":"base64://zRjYZ92/TaqtWroc666wMQ==","rom":{"bytes":"base64://md5://BVt1f9Z97W/m34J/iH68cQ=="},"ram":{"size":64042,"bytes":"base64://zlib://eJztzkENgDAQALDBJeOJAGTghAdW8HQSSHAwP3xxwRJoFbSUv2h1Pco19W68ZIk5Iu5xz23IPGvvDwAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABf9QDDAAbX"}}})", saveStates[2].c_str());
        }

        err = machine->AttachIoController(std::move(controller.value()));
        TEST_ASSERT_FALSE(err);
    }

    static void LoadAndRun(const char* name, const char* expected, const char* extra = nullptr, uint16_t offset = 0)
    {
        bool saveTriggered = false;

        auto err = machine->OnSave([&saveTriggered, expected](const char* actual)
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
            saveTriggered = true;
            return errc::no_error;
        });
        TEST_ASSERT_TRUE(err.value() == errc::no_error || err.value() == errc::not_implemented);

        err = machine->OnLoad([name, extra, offset](char* json, int* jsonLen)
        {
            if (extra == nullptr)
            {
                return LoadProgram(json, jsonLen, R"({"cpu":{"pc":256},"memory":{"rom":{"block":[{"bytes":"%s","offset":0},{"bytes":"%s","offset":256}]}}})", saveAndExit, name);
            }
            else
            {
                return LoadProgram(json, jsonLen, R"({"cpu":{"pc":256},"memory":{"rom":{"block":[{"bytes":"%s","offset":0},{"bytes":"%s","offset":256},{"bytes":"%s","offset":%d}]}}})", saveAndExit, name, extra, offset);
            }
        });
        TEST_ASSERT_FALSE(err);

        err = machine->Run();
        TEST_ASSERT_FALSE(err);
        TEST_ASSERT_TRUE(saveTriggered || machine->OnSave(nullptr).value() == errc::not_implemented);
    }

    static void test_SetNullptrMemoryController()
    {
        auto err = machine->AttachMemoryController(nullptr);
	    TEST_ASSERT_TRUE(err);
        TEST_ASSERT_EQUAL_STRING("An argument supplied to the method is invalid", err.message().c_str());
    }

    static void test_SetNullptrIoController()
    {
        //cppcheck-suppress unknownMacro
        auto err = machine->AttachIoController(nullptr);
        TEST_ASSERT_TRUE(err);
        TEST_ASSERT_EQUAL_STRING("An argument supplied to the method is invalid", err.message().c_str());
    }

    static void test_NegativeISRFrequency()
    {
        //cppcheck-suppress unknownMacro
        auto err = machine->SetOptions(R"({"isrFreq":-1.0})");
        TEST_ASSERT_TRUE(err);
    }

    static void test_MethodsErrorAfterRunCalled()
    {
        auto err = machine->OnLoad([progDir = programsDir.c_str()](char* json, int* jsonLen)
        {
            return LoadProgram(json, jsonLen, R"({"cpu":{"pc":5},"memory":{"rom":{"block":[{"bytes":"%s","offset":0},{"bytes":"%s","offset":5},{"bytes":"%s","offset":50004}]}}})", saveAndExit, nopStart, nopEnd);
        });
        TEST_ASSERT_FALSE(err);

        // Set the resolution so the Run method takes about 1 second to complete therefore allowing subsequent IMachine method calls to return errors
        //cppcheck-suppress unknownMacro
        err = machine->SetOptions(R"({"clockResolution":25000000,"runAsync":true,"isrFreq":0.25})"); // must be async so the Run method returns immediately
        TEST_ASSERT_FALSE(err);

        // We aren't interested in saving, clear the onSave callback
        err = machine->OnSave(nullptr);
        TEST_ASSERT_TRUE(err.value() == errc::no_error || err.value() == errc::not_implemented);
 
        err = machine->Run();
        TEST_ASSERT_FALSE(err);

        // All these methods should return errc::busy
        err = machine->SetOptions(R"({"isrFreq":1})");
        TEST_ASSERT_EQUAL_INT(static_cast<int>(errc::busy), err.value());
        err = machine->AttachMemoryController(nullptr);
        TEST_ASSERT_EQUAL_INT(static_cast<int>(errc::busy), err.value());
        err = machine->AttachIoController(nullptr);
        TEST_ASSERT_EQUAL_INT(static_cast<int>(errc::busy), err.value());
        err = machine->OnLoad([](char*, int*){return errc::no_error;});
        TEST_ASSERT_EQUAL_INT(static_cast<int>(errc::busy), err.value());
        err = machine->OnSave([](const char*){ return errc::no_error; });
        TEST_ASSERT_TRUE(err.value() == static_cast<int>(errc::busy) || err.value() == static_cast<int>(errc::not_implemented));

        // Since we are running async we need to wait for completion
        auto expected = machine->WaitForCompletion();
        TEST_ASSERT_TRUE(expected);
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

    static void RunTestSuite(const char* suiteName, const char* expectedState, const char* expectedMsg, size_t pos)
    {
        // Write to the 'load device', the value doesn't matter (use 0)
        cpmIoController->Write(0xFD, 0, nullptr);
        auto controller = machine->DetachIoController();
        TEST_ASSERT_TRUE(controller);
        // use the cpm io controller for cpm based tests
        auto err = machine->AttachIoController(std::move(cpmIoController));
        TEST_ASSERT_FALSE(err);
        err = machine->SetOptions(R"({"isrFreq":0.02})");
        TEST_ASSERT_FALSE(err);
        //CP/M BDOS print message system call is at memory address 0x05,
        //this will be emulated with the bdosMsg subroutine.
        LoadAndRun((std::string("file://") + programsDir + "/" + suiteName).c_str(), expectedState, bdosMsg, 0x05);
        cpmIoController = std::move(machine->DetachIoController().value());
        TEST_ASSERT_TRUE(cpmIoController);
        auto cpm = static_cast<CpmIoController*>(cpmIoController.get());
        TEST_ASSERT_EQUAL_INT(cpm->Message().find(expectedMsg), pos);
        err = machine->AttachIoController(std::move(controller.value()));
        TEST_ASSERT_FALSE(err);
    }

    static void test_Tst8080()
    {
        RunTestSuite("TST8080.COM", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":170,"b":170,"c":9,"d":170,"e":170,"h":170,"l":170,"s":86},"pc":5,"sp":1981})", "CPU IS OPERATIONAL", 74);
    }

    static void test_8080Pre()
    {
        RunTestSuite("8080PRE.COM", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":9,"d":3,"e":50,"h":1,"l":0,"s":86},"pc":5,"sp":1280})", "8080 Preliminary tests complete", 0);
    }

    static void test_CpuTest()
    {
        RunTestSuite("CPUTEST.COM", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":247,"d":4,"e":23,"h":0,"l":0,"s":70},"pc":5,"sp":12283})", "CPU TESTS OK", 168);
    }

    static void test_8080Exm()
    {
        RunTestSuite("8080EXM.COM", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":10,"c":9,"d":14,"e":30,"h":1,"l":109,"s":70},"pc":5,"sp":54137})", "ERROR", std::string::npos);
    }
} // namespace meen::tests

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
        programsDir = argv[1];
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
// These may take a little while to run on embedded platforms, disabled by default
//        RUN_TEST(meen::tests::test_CpuTest);
//        RUN_TEST(meen::tests::test_8080Exm);
        RUN_TEST(meen::tests::test_MethodsErrorAfterRunCalled);
        RUN_TEST(meen::tests::test_RunTimed);
        RUN_TEST(meen::tests::test_RunTimedAsync);
        err = meen::tests::suiteTearDown(UNITY_END());

        //struct mallinfo m = mallinfo();
        //printf("FREE HEAP: %d\n", GetTotalHeap() - m.uordblks);
    }

    return err;
}