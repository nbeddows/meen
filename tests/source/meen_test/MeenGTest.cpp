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
		static std::shared_ptr<IController> cpmIoController_;
		static std::shared_ptr<MemoryController> memoryController_;
		static std::shared_ptr<IController> testIoController_;
		static std::unique_ptr<IMachine> machine_;

		static void LoadAndRun(const char* name, const char* expected);
		static void Run(bool runAsync);
		static void Load(bool runAsync);
	public:
		static std::string programsDir_;

		static void SetUpTestCase();
		void SetUp();
	};

	std::shared_ptr<IController> MachineTest::cpmIoController_;
	std::shared_ptr<MemoryController> MachineTest::memoryController_;
	std::string MachineTest::programsDir_;
	std::shared_ptr<IController> MachineTest::testIoController_;
	std::unique_ptr<IMachine> MachineTest::machine_;

	void MachineTest::SetUpTestCase()
	{
		// Note that the tests don't require a json string to be set as it just uses the default values,
		// it is used here for demonstation purposes only
		machine_ = Make8080Machine();
		memoryController_ = std::make_shared<MemoryController>();
		cpmIoController_ = std::make_shared<CpmIoController>(static_pointer_cast<IController>(memoryController_));
		testIoController_ = std::make_shared<TestIoController>();

		// Use the default directory if it has not been set by the user
		if(programsDir_.empty() == true)
		{
			programsDir_ = PROGRAMS_DIR;
		}
	}

	void MachineTest::SetUp()
	{
		memoryController_->Clear();
		
		//CP/M Warm Boot is at memory address 0x00, this will be
		//emulated with the exitTest subroutine.
		auto err = memoryController_->Load((programsDir_ + "/exitTest.bin").c_str(), 0x00);
		ASSERT_FALSE(err);
		
		//CP/M BDOS print message system call is at memory address 0x05,
		//this will be emulated with the bdosMsg subroutine.
		err = memoryController_->Load((programsDir_ + "/bdosMsg.bin").c_str(), 0x05);
		ASSERT_FALSE(err);

		machine_->SetMemoryController(memoryController_);
		machine_->SetIoController(testIoController_);
		
		// Set default options
		err = machine_->SetOptions(nullptr);
		EXPECT_FALSE(err);
	}

	void MachineTest::LoadAndRun(const char* name, const char* expected)
	{
		machine_->OnSave([expected](const char* actual)
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
			return errc::no_error;
		});

		auto dir = programsDir_ + name;
		auto err = memoryController_->Load(dir.c_str(), 0x100);
		ASSERT_FALSE(err);

		err = machine_->Run(0x100);
		EXPECT_FALSE(err);
	}

	TEST_F(MachineTest, SetNullptrMemoryController)
	{
		EXPECT_NO_THROW
		(
			auto errc = machine_->SetMemoryController(nullptr);
			EXPECT_TRUE(errc);
			EXPECT_STREQ("An argument supplied to the method is invalid", errc.message().c_str());
		);
	}

	TEST_F(MachineTest, SetNullptrIoController)
	{
		EXPECT_NO_THROW
		(
			//cppcheck-suppress unknownMacro
			auto errc = machine_->SetIoController(nullptr);
			EXPECT_TRUE(errc);
			EXPECT_STREQ("An argument supplied to the method is invalid", errc.message().c_str());
		);
	}

	TEST_F(MachineTest, NegativeISRFrequency)
	{
		EXPECT_NO_THROW
		(
			//cppcheck-suppress unknownMacro
			auto errc = machine_->SetOptions(R"({"isrFreq":-1.0})");
			EXPECT_TRUE(errc)
		);
	}

	TEST_F(MachineTest, MethodsErrorAfterRunCalled)
	{
		//cppcheck-suppress unknownMacro
		// Set the resolution so the Run method takes about 1 second to complete therefore allowing subsequent IMachine method calls to return errors
		auto err = machine_->SetOptions(R"({"clockResolution":25000000,"runAsync":true, "isrFreq":0.25})"); // must be async so the Run method returns immediately
		EXPECT_FALSE(err);

		err = memoryController_->Load((programsDir_ + "nopStart.bin").c_str(), 0x04);
		ASSERT_FALSE(err);
		
		err = memoryController_->Load((programsDir_ + "nopEnd.bin").c_str(), 0xC353);
		ASSERT_FALSE(err);

		EXPECT_NO_THROW
		(
			// We aren't interested in saving, clear the onSave callback
			err = machine_->OnSave(nullptr);
			EXPECT_TRUE(err.value() == errc::no_error || err.value() == errc::json_config);

			err = machine_->Run(0x04);
			EXPECT_FALSE(err);

			// All these methods should return errors
			err = machine_->SetOptions(R"({"isrFreq":1})");
			EXPECT_TRUE(err);
			err = machine_->SetMemoryController(memoryController_);
			EXPECT_TRUE(err);
			err = machine_->SetIoController(testIoController_);
			EXPECT_TRUE(err);
			err = machine_->OnLoad([](char* json, int* jsonLen){ return errc::no_error; });
			EXPECT_TRUE(err);
			err = machine_->OnSave([](const char*){ return errc::no_error; } );
			EXPECT_TRUE(err);

			// Since we are running async we need to wait for completion
			machine_->WaitForCompletion();

			// We are now no longer running, all these methods should not return errors
			err = machine_->SetOptions(R"({"isrFreq":1})");
			EXPECT_FALSE(err);
			err = machine_->SetMemoryController(memoryController_);
			EXPECT_FALSE(err);
			err = machine_->SetIoController(testIoController_);
			EXPECT_FALSE(err);
			err = machine_->OnLoad([](char* json, int* jsonLen){ return errc::no_error; });
			EXPECT_TRUE(err.value() == errc::no_error || err.value() == errc::json_config);
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

		// Run a program that should take a second to complete
		// (in actual fact it's 2000047 ticks, 47 ticks over a second.
		// We need to be as close a possible to 2000000 ticks without
		// going under so the cpu sleeps at the end
		// of the program so it maintains sync. It's never going to
		// be perfect, but its close enough for testing purposes).
		err = memoryController_->Load((programsDir_ + "nopStart.bin").c_str(), 0x04);
		ASSERT_FALSE(err);

		err = memoryController_->Load((programsDir_ + "nopEnd.bin").c_str(), 0xC353);
		ASSERT_FALSE(err);

		// 25 millisecond resolution, service interrupts every 8.25 milliseconds
		err = machine_->SetOptions(R"({"clockResolution":25000000,"isrFreq":0.25})");
		EXPECT_FALSE(err);

		err = machine_->Run(0x04);
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

			auto cpmIoController = static_pointer_cast<CpmIoController>(cpmIoController_);
			// Trigger a save when the 3000th cycle has executed.
			cpmIoController->SaveStateOn(3000);
			// Call the out instruction
			memoryController_->Write(0x00FE, 0xD3);
			// The data to write to the controller that will trigger the ISR::Load interrupt
			memoryController_->Write(0x00FF, 0xFD);

			err = memoryController_->Load((programsDir_ + "/TST8080.COM").c_str(), 0x100);
			ASSERT_FALSE(err);
			// Set the rom/ram offsets for tst8080, note that tst8080 uses 256 bytes of stack space
			// located at the end of the program so this will make up the ram size since the program
			// never writes beyond this.
			err = machine_->SetOptions(R"({"rom":{"file":[{"offset":0,"size":1727}]},"ram":{"block":[{"offset":1727,"size":256}]}})");
			EXPECT_FALSE(err);
			machine_->SetIoController(cpmIoController_);

			err = machine_->Run(0x0100);
			EXPECT_FALSE(err);

			auto ex = machine_->WaitForCompletion();
			EXPECT_TRUE(ex);

			EXPECT_EQ(74, cpmIoController->Message().find("CPU IS OPERATIONAL"));

			// Disable triggering a save from this controller so the other cpm tests will pass.
			// Needs to be done before the next Run call so the async version of this test won't
			// trigger a spurious ISR::Save interurpt if the ISR::Load interrupt takes too long
			// to process
			cpmIoController->SaveStateOn(-1);

			// run it again, but this time trigger the load interrupt
			err = machine_->Run(0x00FE);
			EXPECT_FALSE(err);

			ex = machine_->WaitForCompletion();
			EXPECT_TRUE(ex);

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