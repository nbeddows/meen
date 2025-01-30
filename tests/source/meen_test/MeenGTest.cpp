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

#include <gtest/gtest.h>
#include <memory>
#ifdef ENABLE_NLOHMANN_JSON
#include <nlohmann/json.hpp>
#else
#include <ArduinoJson.h>
#endif
#include <stdarg.h>

#include "meen/Error.h"
#include "meen/IController.h"
#include "meen/IMachine.h"
#include "meen/MachineFactory.h"
#include "test_controllers/MemoryController.h"
#include "test_controllers/TestIoController.h"
#include "test_controllers/CpmIoController.h"

namespace meen::Tests
{
	class MachineTest : public testing::Test
	{
	protected:
		static IControllerPtr cpmIoController_;
		static std::unique_ptr<IMachine> machine_;

		static void LoadAndRun(const char* name, const char* expected, const char* extra = nullptr, uint16_t extraOffset = 0);
		static void Run(bool runAsync);
		static void Load(bool runAsync);
	    static void RunTestSuite(const char* suiteName, const char* expectedState, const char* expectedMsg, int pos);
		static errc LoadProgram (char* json, int* jsonLen, const char* fmt, ...);

	public:
		static std::string programsDir_;

		static void SetUpTestCase();
		void SetUp();
	};

	IControllerPtr MachineTest::cpmIoController_;
	std::string MachineTest::programsDir_;
	std::unique_ptr<IMachine> MachineTest::machine_;

	void MachineTest::SetUpTestCase()
	{
		machine_ = Make8080Machine();
		cpmIoController_ = IControllerPtr(new CpmIoController());

		// Use the default directory if it has not been set by the user
		if(programsDir_.empty() == true)
		{
			programsDir_ = PROGRAMS_DIR;
		}

		auto err = machine_->AttachMemoryController(IControllerPtr(new MemoryController()));
		EXPECT_FALSE(err);
		
		err = machine_->AttachIoController(IControllerPtr(new TestIoController()));
		EXPECT_FALSE(err);
	}

	void MachineTest::SetUp()
	{

		// At the start of each test we need to trigger a load interrupt so the next test program will load ... need to be careful with tests that don't require a load interrupt, may need to reset the load handler
		auto controller = machine_->DetachIoController();
		ASSERT_TRUE(controller);
		// Write to the 'load device', the value doesn't matter (use 0)
		controller.value()->Write(0xFD, 0, nullptr);
		auto err = machine_->AttachIoController(std::move(controller.value()));
		EXPECT_FALSE(err);

		// Set default options
		err = machine_->SetOptions(nullptr);
		EXPECT_FALSE(err);
	}

	errc MachineTest::LoadProgram(char* json, int* jsonLen, const char* fmt, ...)
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

	void MachineTest::LoadAndRun(const char* name, const char* expected, const char* extra, uint16_t offset)
	{
		bool saveTriggered = false;

		auto err = machine_->OnSave([st = &saveTriggered, expected](const char* actual)
		{
			std::string actualStr;
			std::string expectedStr;
#ifdef ENABLE_NLOHMANN_JSON
			auto actualJson = nlohmann::json::parse(actual, nullptr, false);
			EXPECT_FALSE(actualJson.is_discarded());
			auto expectedJson = nlohmann::json::parse(expected, nullptr, false);
			EXPECT_FALSE(expectedJson.is_discarded());
			expectedStr = expectedJson.dump();
			actualStr = actualJson["cpu"].dump();
#else
			JsonDocument actualJson;
			JsonDocument expectedJson;
			auto err = deserializeJson(actualJson, actual);
			EXPECT_FALSE(err);
			err = deserializeJson(expectedJson, expected);
			EXPECT_FALSE(err);
			serializeJson(actualJson["cpu"], actualStr);
			serializeJson(expectedJson, expectedStr);
#endif
			EXPECT_STREQ(expectedStr.c_str(), actualStr.c_str());
			*st = true;
			return errc::no_error;
		});
		EXPECT_FALSE(err);


		err = machine_->OnLoad([progDir = programsDir_.c_str(), progName = std::move(name), ex = std::move(extra), off = offset](char* json, int* jsonLen)
		{
			if (ex == nullptr)
			{
				return LoadProgram(json, jsonLen, R"({"cpu":{"pc":256},"memory":{"rom":{"block":[{"bytes":"file://%s/exitTest.bin","offset":0},{"bytes":"file://%s/%s","offset":256}]}}})", progDir, progDir, progName);
			}
			else
			{
				return LoadProgram(json, jsonLen, R"({"cpu":{"pc":256},"memory":{"rom":{"block":[{"bytes":"file://%s/exitTest.bin","offset":0},{"bytes":"file://%s/%s","offset":256},{"bytes":"file://%s/%s","offset":%d}]}}})", progDir, progDir, progName, progDir, ex, off);
			}
		});
		EXPECT_FALSE(err);

		err = machine_->Run(0x00);
		EXPECT_FALSE(err);
		EXPECT_TRUE(saveTriggered);
	}

    void MachineTest::RunTestSuite(const char* suiteName, const char* expectedState, const char* expectedMsg, int pos)
    {
		// Write to the 'load device', the value doesn't matter (use 0)
		cpmIoController_->Write(0xFD, 0, nullptr);
		// Cache the defacto test io controller 
		auto controller = machine_->DetachIoController();
		ASSERT_TRUE(controller);
		// use the cpm io controller for cpm based tests
		auto err = machine_->AttachIoController(std::move(cpmIoController_));
		EXPECT_FALSE(err);
		err = machine_->SetOptions(R"({"isrFreq":0.02})");
		EXPECT_FALSE(err);
		//CP/M BDOS print message system call is at memory address 0x05,
		//this will be emulated with the bdosMsg subroutine.
		LoadAndRun(suiteName, expectedState, "bdosMsg.bin", 0x05);
        cpmIoController_ = std::move(machine_->DetachIoController().value());
        ASSERT_TRUE(cpmIoController_);
        auto cpm = static_cast<CpmIoController*>(cpmIoController_.get());
		EXPECT_EQ(cpm->Message().find(expectedMsg), pos);
		// Restore the defacto test io controller
		err = machine_->AttachIoController(std::move(controller.value()));
		EXPECT_FALSE(err);
    }

	TEST_F(MachineTest, AttachNullptrMemoryController)
	{
		EXPECT_NO_THROW
		(
			auto err = machine_->AttachMemoryController(nullptr);
			EXPECT_EQ(errc::invalid_argument, err.value());
		);
	}

	TEST_F(MachineTest, AttachNullptrIoController)
	{
		EXPECT_NO_THROW
		(
			//cppcheck-suppress unknownMacro
			auto err = machine_->AttachIoController(nullptr);
			EXPECT_EQ(errc::invalid_argument, err.value());
		);
	}

	TEST_F(MachineTest, NegativeISRFrequency)
	{
		EXPECT_NO_THROW
		(
			//cppcheck-suppress unknownMacro
			auto err = machine_->SetOptions(R"({"isrFreq":-1.0})");
			EXPECT_EQ(errc::json_config, err.value());
		);
	}

	TEST_F(MachineTest, MethodsErrorAfterRunCalled)
	{
		EXPECT_NO_THROW
		(
			auto err = machine_->OnLoad([progDir = programsDir_.c_str()](char* json, int* jsonLen)
			{
				return LoadProgram (json, jsonLen, R"({"cpu":{"pc":5},"memory":{"rom":{"block":[{"bytes":"file://%s/exitTest.bin","offset":0},{"bytes":"file://%s/nopStart.bin","offset":5},{"bytes":"file://%s/nopEnd.bin","offset":50004}]}}})", progDir, progDir, progDir);
			});
			EXPECT_FALSE(err);

			//cppcheck-suppress unknownMacro
			// Set the resolution so the Run method takes about 1 second to complete therefore allowing subsequent IMachine method calls to return errors
			err = machine_->SetOptions(R"({"clockResolution":25000000,"runAsync":true, "isrFreq":0.25})"); // must be async so the Run method returns immediately
			EXPECT_FALSE(err);

			// We aren't interested in saving, clear the onSave callback
			err = machine_->OnSave(nullptr);
			EXPECT_TRUE(err.value() == errc::no_error || err.value() == errc::not_implemented);

			err = machine_->Run(0x00);
			EXPECT_FALSE(err);

			// All these methods should return busy
			err = machine_->SetOptions(R"({"isrFreq":1})");
			EXPECT_EQ(errc::busy, err.value());
			err = machine_->AttachMemoryController(nullptr);
			EXPECT_EQ(errc::busy, err.value());
			err = machine_->AttachIoController(nullptr);
			EXPECT_EQ(errc::busy, err.value());
			err = machine_->OnLoad([](char*, int*){ return errc::no_error; });
			EXPECT_EQ(errc::busy, err.value());
			err = machine_->OnSave([](const char*){ return errc::no_error; });
			EXPECT_EQ(errc::busy, err.value());

			// Since we are running async we need to wait for completion
			machine_->WaitForCompletion();
		);
	}

	void MachineTest::Run(bool runAsync)
	{
		std::error_code err;
		int64_t nanos = 0;

		if (runAsync == true)
		{
			err = machine_->SetOptions(R"({"runAsync":true})");
			EXPECT_FALSE(err);
		}

		err = machine_->OnLoad([progDir = programsDir_.c_str()](char* json, int* jsonLen)
		{
			return LoadProgram (json, jsonLen, R"({"cpu":{"pc":5},"memory":{"rom":{"block":[{"bytes":"file://%s/exitTest.bin","offset":0},{"bytes":"file://%s/nopStart.bin","offset":5},{"bytes":"file://%s/nopEnd.bin","offset":50004}]}}})", progDir, progDir, progDir);
		});
		EXPECT_FALSE(err);

		// 25 millisecond resolution, service interrupts every 6.25 milliseconds
		err = machine_->SetOptions(R"({"clockResolution":25000000,"isrFreq":0.25})");
		EXPECT_FALSE(err);

		err = machine_->Run(0x00);
		EXPECT_FALSE(err);

// Use std::expected monadics if they are supported
#if ((defined __GNUC__ && __GNUC__ >= 13) || (defined _MSC_VER && _MSC_VER >= 1706))
		nanos += machine_->WaitForCompletion().or_else([](std::error_code ec)
		{
			// We want to force a failure here, ec should be non zero
			EXPECT_FALSE(ec);
			// We failed, return back a 0 run time
			return std::expected<uint64_t, std::error_code>(0);
		}).value();
#else
		auto ex = machine_->WaitForCompletion();
		EXPECT_TRUE(ex);

		nanos += ex.value();
#endif
		auto error = nanos - 1000000000;
		// Allow an average 500 micros of over sleep error
		EXPECT_EQ(true, error >= 0 && error <= 500000);
	}

	TEST_F(MachineTest, RunTimed)
	{
		Run(false);
	}

	TEST_F(MachineTest, RunTimedAsync)
	{
		Run(true);
	}

#if 0
	void MachineTest::Load(bool runAsync)
	{
		EXPECT_NO_THROW
		(
			std::vector<std::string> saveStates;
			auto err = machine_->OnSave([&](const char* json)
			{
				saveStates.emplace_back(json);
				return errc::no_error;
			});

			if(err.value() == errc::json_config)
			{
				// not implemented, skip the test
				GTEST_SKIP() << "Machine::OnSave not supported";
			}

			// 0 - mid program save state, 1 and 2 - end of program save states
			err = machine_->OnLoad([&](char* json, int* jsonLen)
			{
				if(json == nullptr)
				{
					*jsonLen = saveStates[0].length();
				}
				else
				{
					memcpy(json, saveStates[0].c_str(), *jsonLen);
				}

				return errc::no_error;
			});

			if(err.value() == errc::json_config)
			{
				// not implemented, skip the test
				GTEST_SKIP() << "Machine::OnLoad not supported";
			}

			if (runAsync == true)
			{
				err = machine_->SetOptions(R"({"runAsync":true,"loadAsync":false,"saveAsync":true})");
				EXPECT_FALSE(err);
			}

			auto controller = machine_->DetachMemoryController();
			ASSERT_TRUE(controller);

			// Call the out instruction
			controller.value()->Write(0x00FE, 0xD3, nullptr);
			// The data to write to the controller that will trigger the ISR::Load interrupt
			controller.value()->Write(0x00FF, 0xFD, nullptr);

			auto memoryController = static_cast<MemoryController*>(controller.value().get());
			err = memoryController->Load((programsDir_ + "/TST8080.COM").c_str(), 0x100);
			ASSERT_FALSE(err);

			err = machine_->AttachMemoryController(std::move(controller.value()));
			EXPECT_FALSE(err);

			auto cpmIoController = static_cast<CpmIoController*>(cpmIoController_.get());
			// Trigger a save when the 3000th cycle has executed.
			cpmIoController->SaveStateOn(3000);

			// Detach the defacto test io controller
			controller = machine_->DetachIoController();
			ASSERT_TRUE(controller);

			// Attach the cpm io controller
			err = machine_->AttachIoController(std::move(cpmIoController_));
			EXPECT_FALSE(err);

			// Set the rom/ram offsets for tst8080, note that tst8080 uses 256 bytes of stack space
			// located at the end of the program so this will make up the ram size since the program
			// never writes beyond this.
			err = machine_->SetOptions(R"({"rom":{"file":[{"offset":0,"size":1727}]},"ram":{"block":[{"offset":1727,"size":256}]}})");
			EXPECT_FALSE(err);

			err = machine_->Run(0x0100);
			EXPECT_FALSE(err);

			auto ex = machine_->WaitForCompletion();
			EXPECT_TRUE(ex);

			cpmIoController_  = std::move(machine_->DetachIoController().value());
			ASSERT_TRUE(cpmIoController_);

			cpmIoController = static_cast<CpmIoController*>(cpmIoController_.get());
			ASSERT_TRUE(cpmIoController);

			EXPECT_EQ(74, cpmIoController->Message().find("CPU IS OPERATIONAL"));

			// Disable triggering a save from this controller so the other cpm tests will pass.
			// Needs to be done before the next Run call so the async version of this test won't
			// trigger a spurious ISR::Save interurpt if the ISR::Load interrupt takes too long
			// to process
			cpmIoController->SaveStateOn(-1);

			err = machine_->AttachIoController(std::move(cpmIoController_));
			EXPECT_FALSE(err);

			// run it again, but this time trigger the load interrupt
			err = machine_->Run(0x00FE);
			EXPECT_FALSE(err);

			ex = machine_->WaitForCompletion();
			EXPECT_TRUE(ex);

			cpmIoController_ = std::move(machine_->DetachIoController().value());
			ASSERT_TRUE(cpmIoController_);

			cpmIoController = static_cast<CpmIoController*>(cpmIoController_.get());
			ASSERT_TRUE(cpmIoController);

			// Since we are not saving/loading the io state the contents of the message buffer can
			// be in one of two states depending on how long the OnLoad initiation handler took to complete.
			auto pos = cpmIoController->Message().find("CPU IS OPERATIONAL");

			// Currently we are not saving the state of the io (do we need to?????)
			// This can cause variable output as discussed below
			if (runAsync == true)
			{
				// If the OnLoad initiation handler was quick to complete (sub 150 ticks) the preamble message would
				// not have been written to the message string and the success message should be found at pos 3, otherwise
				// the preamble message was written and it should be found at pos 74
				EXPECT_TRUE(3 == pos || 74 == pos);
			}
			else
			{
				// Since we loaded mid program the message from the tests won't contain the premable
				// (since we are not saving/loading the io state), just the result,
				// hence we should find the success message earlier in the message string.
				EXPECT_EQ(3, pos);
			}

			// When we are in the middle of a save when another save is requested it will be dropped.
			// This may or may not happen depending on how fast the first save takes to complete.
			ASSERT_TRUE(saveStates.size() == 3 || saveStates.size() == 2);
			EXPECT_STREQ(R"({"cpu":{"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":19,"b":19,"c":0,"d":19,"e":0,"h":19,"l":0,"s":86},"pc":1236,"sp":1981},"memory":{"uuid":"zRjYZ92/TaqtWroc666wMQ==","rom":"JXg8/M+WvmCGVMmH7xr/0g==","ram":{"encoder":"base64","compressor":"zlib","size":256,"bytes":"eJwLZRhJQJqZn5mZ+TvTa6b7TJeZjjIxMAAAfY0E7w=="}}})", saveStates[0].c_str());
			EXPECT_STREQ(R"({"cpu":{"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":170,"b":170,"c":9,"d":170,"e":170,"h":170,"l":170,"s":86},"pc":2,"sp":1981},"memory":{"uuid":"zRjYZ92/TaqtWroc666wMQ==","rom":"JXg8/M+WvmCGVMmH7xr/0g==","ram":{"encoder":"base64","compressor":"zlib","size":256,"bytes":"eJw7w2ZczrCXnWFkAGlmfmZm5u9MYauCGFet2sXGwAAAYNgG1w=="}}})", saveStates[1].c_str());

			if (saveStates.size() == 3)
			{
				EXPECT_STREQ(R"({"cpu":{"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":170,"b":170,"c":9,"d":170,"e":170,"h":170,"l":170,"s":86},"pc":2,"sp":1981},"memory":{"uuid":"zRjYZ92/TaqtWroc666wMQ==","rom":"JXg8/M+WvmCGVMmH7xr/0g==","ram":{"encoder":"base64","compressor":"zlib","size":256,"bytes":"eJw7w2ZczrCXnWFkAGlmfmZm5u9MYauCGFet2sXGwAAAYNgG1w=="}}})", saveStates[2].c_str());
			}

			// re-attach the defacto test io controller
			err = machine_->AttachIoController(std::move(controller.value()));
			EXPECT_FALSE(err);
		);
	}

	TEST_F(MachineTest, OnLoad)
	{
		for (int i = 0; i < 50; i++)
		{
			Load(false);
		}
	}

	TEST_F(MachineTest, OnLoadAsync)
	{
		for (int i = 0; i < 50; i++)
		{
			Load(true);
		}
	}
#endif

	TEST_F(MachineTest, Tst8080)
    {
        RunTestSuite("TST8080.COM", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":170,"b":170,"c":9,"d":170,"e":170,"h":170,"l":170,"s":86},"pc":5,"sp":1981})", "CPU IS OPERATIONAL", 74);
	}

    TEST_F(MachineTest, 8080Pre)
	{
        RunTestSuite("8080PRE.COM", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":9,"d":3,"e":50,"h":1,"l":0,"s":86},"pc":5,"sp":1280})", "8080 Preliminary tests complete", 0);
    }

    TEST_F(MachineTest, CpuTest)
	{
		RunTestSuite("CPUTEST.COM", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":247,"d":4,"e":23,"h":0,"l":0,"s":70},"pc":5,"sp":12283})", "CPU TESTS OK", 168);
    }

    TEST_F(MachineTest, 8080Exm)
	{
        RunTestSuite("8080EXM.COM", R"({"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":10,"c":9,"d":14,"e":30,"h":1,"l":109,"s":70},"pc":5,"sp":54137})", "ERROR", std::string::npos);
    }

	#include "8080Test.cpp"
} // namespace meen::Tests

int main(int argc, char** argv)
{
	std::cout << "Running main() from MachineTest.cpp" << std::endl;
	testing::InitGoogleTest(&argc, argv);

	if (argc > 1)
	{
		meen::Tests::MachineTest::programsDir_ = argv[1];
	}

	return RUN_ALL_TESTS();
}