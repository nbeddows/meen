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
		// A base64 encoded code fragment that is loaded at address 0x0000 (for test suite compatibility) which saves the current machine state, powers off the machine, then halts the cpu.
		static inline const char* saveAndExit = "base64://0/7T/3Y";
		// A base64 encoded code fragment that initialises the 'a' register to 10 and is loaded at address 0x0005. Used in conjunction with nopEnd to run the timing test
		static inline const char* nopStart = "base64://Pgo=";
		// A base64 encoded code fragment that decrements the 'a' register by 1, jumps to address 0x0005 if 'a' is non-zero else address 0x0000.
		static inline const char* nopEnd = "base64://PcIHAMMAAA";
		// A base64 encoded code fragment that emulates cp/m bdos function 4 - raw console output.  
		static inline const char* bdosMsg = "base64://9XnTAP4CyhEAetMBe9MC8ck=";

		static void LoadAndRun(const char* name, const char* expected, const char* extra = nullptr, uint16_t extraOffset = 0);
		static void Run(bool runAsync);
		static void Load(bool runAsync);
	    static void RunTestSuite(const char* suiteName, const char* expectedState, const char* expectedMsg, size_t pos);
		static errc LoadProgram (char* json, int* jsonLen, const char* fmt, ...);
		static std::string ReadCpmIoControllerBuffer();

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

	// todo: the returned 'json' needs to be prefixed with a protcol so it can be read from file:// for example
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

		auto err = machine_->OnSave([&saveTriggered, expected](const char* actual)
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
			saveTriggered = true;
			return errc::no_error;
		});
		EXPECT_TRUE(err.value() == errc::no_error || err.value() == errc::not_implemented);

		err = machine_->OnLoad([name, extra, offset](char* json, int* jsonLen)
		{
			if (extra == nullptr)
			{
				return LoadProgram(json, jsonLen, R"(json://{"cpu":{"pc":256},"memory":{"rom":{"block":[{"bytes":"%s","offset":0},{"bytes":"%s","offset":256}]}}})", saveAndExit, name);
			}
			else
			{
				return LoadProgram(json, jsonLen, R"(json://{"cpu":{"pc":256},"memory":{"rom":{"block":[{"bytes":"%s","offset":0},{"bytes":"%s","offset":256},{"bytes":"%s","offset":%d}]}}})", saveAndExit, name, extra, offset);
			}
		});
		EXPECT_FALSE(err);

		err = machine_->Run();
		EXPECT_FALSE(err);
		EXPECT_TRUE(saveTriggered || machine_->OnSave(nullptr).value() == errc::not_implemented);
	}

	std::string MachineTest::ReadCpmIoControllerBuffer()
	{
		std::string message;
		uint8_t byte = 0x04; // ascii end of transmission

		do
		{
			byte = cpmIoController_->Read(0, nullptr);
		
			if (byte != 0x04)
			{
				message.push_back(byte);
			}
		}
		while (byte != 0x04);

		return message;
	}

    void MachineTest::RunTestSuite(const char* suiteName, const char* expectedState, const char* expectedMsg, size_t pos)
    {
		// Write to the 'load device', the value doesn't matter (use 0)
		cpmIoController_->Write(0xFD, 0, nullptr);
		// Cache the defacto test io controller
		auto controller = machine_->DetachIoController();
		ASSERT_TRUE(controller);
		// use the cpm io controller for cpm based tests
		auto err = machine_->AttachIoController(std::move(cpmIoController_));
		EXPECT_FALSE(err);
		err = machine_->SetOptions(R"(json://{"isrFreq":60})");
		EXPECT_FALSE(err);
		//CP/M BDOS print message system call is at memory address 0x05,
		//this will be emulated with the bdosMsg subroutine.
		LoadAndRun((std::string("file://") + programsDir_ + "/" + suiteName).c_str(), expectedState, bdosMsg, 0x05);
		cpmIoController_ = std::move(machine_->DetachIoController().value());
		ASSERT_TRUE(cpmIoController_);
		EXPECT_EQ(ReadCpmIoControllerBuffer().find(expectedMsg), pos);
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
			auto err = machine_->SetOptions(R"(json://{"isrFreq":-1.0})");
			EXPECT_EQ(errc::json_config, err.value());
		);
	}

	TEST_F(MachineTest, MethodsErrorAfterRunCalled)
	{
		EXPECT_NO_THROW
		(
			auto err = machine_->OnLoad([progDir = programsDir_.c_str()](char* json, int* jsonLen)
			{
				return LoadProgram (json, jsonLen, R"(json://{"cpu":{"pc":5},"memory":{"rom":{"block":[{"bytes":"%s","offset":0},{"bytes":"%s","offset":5},{"bytes":"%s","offset":50004}]}}})", saveAndExit, nopStart, nopEnd);
			});
			EXPECT_FALSE(err);

			// Set the resolution so the Run method takes about 1 second to complete therefore allowing subsequent IMachine method calls to return errors
			//cppcheck-suppress unknownMacro
			err = machine_->SetOptions(R"(json://{"clockSamplingFreq":40,"runAsync":true, "isrFreq":60})"); // must be async so the Run method returns immediately
			EXPECT_FALSE(err);

			// We aren't interested in saving, clear the onSave callback
			err = machine_->OnSave(nullptr);
			EXPECT_TRUE(err.value() == errc::no_error || err.value() == errc::not_implemented);

			err = machine_->Run();
			EXPECT_FALSE(err);

			// All these methods should return busy
			err = machine_->SetOptions(R"(json://{"isrFreq":1})");
			EXPECT_EQ(errc::busy, err.value());
			err = machine_->AttachMemoryController(nullptr);
			EXPECT_EQ(errc::busy, err.value());
			err = machine_->AttachIoController(nullptr);
			EXPECT_EQ(errc::busy, err.value());
			err = machine_->OnLoad([](char*, int*){ return errc::no_error; });
			EXPECT_EQ(errc::busy, err.value());
			err = machine_->OnSave([](const char*){ return errc::no_error; });
			EXPECT_TRUE(err.value() == errc::busy || err.value() == errc::not_implemented);

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
			err = machine_->SetOptions(R"(json://{"runAsync":true})");
			EXPECT_FALSE(err);
		}

		err = machine_->OnLoad([progDir = programsDir_.c_str()](char* json, int* jsonLen)
		{
			return LoadProgram (json, jsonLen, R"(json://{"cpu":{"pc":5},"memory":{"rom":{"block":[{"bytes":"%s","offset":0},{"bytes":"%s","offset":5},{"bytes":"%s","offset":50004}]}}})", saveAndExit, nopStart, nopEnd);
		});
		EXPECT_FALSE(err);

		// Sample the host clock 40 times per second, giving a meen clock tick a resolution of 25 milliseconds
		// Service interrupts 60 times per meen cpu clock rate. For an i8080 running at 2Mhz, this would service interrupts every 40000 ticks.
		err = machine_->SetOptions(R"(json://{"clockSamplingFreq":40,"isrFreq":60})");
		EXPECT_FALSE(err);

		err = machine_->Run();
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

	void MachineTest::Load(bool runAsync)
	{
		EXPECT_NO_THROW
		(
			int loadIndex = 0;
			std::vector<std::string> saveStates;
			auto err = machine_->OnSave([&](const char* json)
			{
				saveStates.emplace_back(json);
				return errc::no_error;
			});

			if(err.value() == errc::not_implemented)
			{
				// not implemented, skip the test
				GTEST_SKIP() << "Machine::OnSave not supported";
			}

			err = machine_->OnLoad([&saveStates, &loadIndex, progDir = programsDir_.c_str()](char* json, int* jsonLen)
			{
				auto err = errc::no_error;

				switch (loadIndex)
				{
					// Tst8080 test file includes a small amount of stack space
					// located at the end of the program. This ram needs to
					// be subtracted from the total size of the file,
					// hence an explicit setting of the test file size.
					case 0:
						err = LoadProgram(json, jsonLen, R"(json://{"cpu":{"pc":256},"memory":{"rom":{"block":[{"bytes":"%s","offset":0},{"bytes":"%s","offset":5},{"bytes":"file://%s/TST8080.COM","offset":256,"size":1471}]}}})", saveAndExit, bdosMsg, progDir);
						break;
					case 1:
						EXPECT_FALSE(saveStates.empty());
						
						if (saveStates.empty() == false)
						{
							// 0 - mid program save state, 1 and 2 - end of program save states
							err = LoadProgram(json, jsonLen, (std::string("json://") + saveStates[0]).c_str());
						}
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
				GTEST_SKIP() << "Machine::OnLoad not supported";
			}

			if (runAsync == true)
			{
				err = machine_->SetOptions(R"(json://{"runAsync":true,"loadAsync":false,"saveAsync":true})");
				EXPECT_FALSE(err);
			}

			// Write to the 'load device', the value doesn't matter (use 0)
			cpmIoController_->Write(0xFD, 0, nullptr);

			auto cpmIoController = static_cast<CpmIoController*>(cpmIoController_.get());
			// Trigger a save when the 3000th cycle has executed.
			cpmIoController->SaveStateOn(3000);

			// Detach the defacto test io controller
			auto controller = machine_->DetachIoController();
			ASSERT_TRUE(controller);

			// Attach the cpm io controller
			err = machine_->AttachIoController(std::move(cpmIoController_));
			EXPECT_FALSE(err);

			err = machine_->Run();
			EXPECT_FALSE(err);

			auto ex = machine_->WaitForCompletion();
			EXPECT_TRUE(ex);

			cpmIoController_  = std::move(machine_->DetachIoController().value());
			ASSERT_TRUE(cpmIoController_);

			EXPECT_EQ(74, ReadCpmIoControllerBuffer().find("CPU IS OPERATIONAL"));

			// Write to the 'load device', the value doesn't matter (use 0)
			cpmIoController_->Write(0xFD, 0, nullptr);

			cpmIoController = static_cast<CpmIoController*>(cpmIoController_.get());
			ASSERT_TRUE(cpmIoController);

			// Disable triggering a save from this controller so the other cpm tests will pass.
			// Needs to be done before the next Run call so the async version of this test won't
			// trigger a spurious ISR::Save interurpt if the ISR::Load interrupt takes too long
			// to process
			cpmIoController->SaveStateOn(-1);

			err = machine_->AttachIoController(std::move(cpmIoController_));
			EXPECT_FALSE(err);

			// run it again, but this time trigger the load interrupt
			err = machine_->Run();
			EXPECT_FALSE(err);

			ex = machine_->WaitForCompletion();
			EXPECT_TRUE(ex);

			cpmIoController_ = std::move(machine_->DetachIoController().value());
			ASSERT_TRUE(cpmIoController_);

			// Since we are not saving/loading the io state the contents of the message buffer can
			// be in one of two states depending on how long the OnLoad initiation handler took to complete.
			auto pos = ReadCpmIoControllerBuffer().find("CPU IS OPERATIONAL");

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
			EXPECT_STREQ(R"({"cpu":{"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":19,"b":19,"c":0,"d":19,"e":0,"h":19,"l":0,"s":86},"pc":1236,"sp":1981},"memory":{"uuid":"base64://zRjYZ92/TaqtWroc666wMQ==","rom":{"bytes":"base64://md5://BVt1f9Z97W/m34J/iH68cQ=="},"ram":{"size":64042,"bytes":"base64://zlib://eJztzlENgDAQBbDlnQAETBeSpwABCEDAfnHBktEqaGt/ca4OfKrXUVUzT+5cGVn9AQAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAGBXL4n+BO8="}}})", saveStates[0].c_str());
			EXPECT_STREQ(R"({"cpu":{"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":170,"b":170,"c":9,"d":170,"e":170,"h":170,"l":170,"s":86},"pc":2,"sp":1981},"memory":{"uuid":"base64://zRjYZ92/TaqtWroc666wMQ==","rom":{"bytes":"base64://md5://BVt1f9Z97W/m34J/iH68cQ=="},"ram":{"size":64042,"bytes":"base64://zlib://eJztzkENgDAQALDBJeOJAGTghAdW8HQSSHAwP3xxwRJoFbSUv2h1Pco19W68ZIk5Iu5xz23IPGvvDwAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABf9QDDAAbX"}}})", saveStates[1].c_str());

			if (saveStates.size() == 3)
			{
				EXPECT_STREQ(R"({"cpu":{"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":170,"b":170,"c":9,"d":170,"e":170,"h":170,"l":170,"s":86},"pc":2,"sp":1981},"memory":{"uuid":"base64://zRjYZ92/TaqtWroc666wMQ==","rom":{"bytes":"base64://md5://BVt1f9Z97W/m34J/iH68cQ=="},"ram":{"size":64042,"bytes":"base64://zlib://eJztzkENgDAQALDBJeOJAGTghAdW8HQSSHAwP3xxwRJoFbSUv2h1Pco19W68ZIk5Iu5xz23IPGvvDwAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABf9QDDAAbX"}}})", saveStates[2].c_str());
			}

			// re-attach the defacto test io controller
			err = machine_->AttachIoController(std::move(controller.value()));
			EXPECT_FALSE(err);
		);
	}

	TEST_F(MachineTest, OnLoad)
	{
		for (int i = 0; i < 10; i++)
		{
			Load(false);
		}
	}

	TEST_F(MachineTest, OnLoadAsync)
	{
		for (int i = 0; i < 10; i++)
		{
			Load(true);
		}
	}

	TEST_F(MachineTest, Tst8080)
	{
		RunTestSuite("TST8080.COM", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":170,"b":170,"c":9,"d":170,"e":170,"h":170,"l":170,"s":86},"pc":5,"sp":1981})", "CPU IS OPERATIONAL", 74);
	}

	TEST_F(MachineTest, 8080Pre)
	{
		RunTestSuite("8080PRE.COM", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":9,"d":3,"e":50,"h":1,"l":0,"s":86},"pc":5,"sp":1280})", "8080 Preliminary tests complete", 0);
	}

	TEST_F(MachineTest, CpuTest)
	{
		RunTestSuite("CPUTEST.COM", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":247,"d":4,"e":23,"h":0,"l":0,"s":70},"pc":5,"sp":12283})", "CPU TESTS OK", 168);
	}

	TEST_F(MachineTest, 8080Exm)
	{
		RunTestSuite("8080EXM.COM", R"({"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":10,"c":9,"d":14,"e":30,"h":1,"l":109,"s":70},"pc":5,"sp":54137})", "ERROR", std::string::npos);
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