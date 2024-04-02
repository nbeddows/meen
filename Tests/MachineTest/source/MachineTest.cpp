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

import <memory>;
import MemoryController;
import TestIoController;

#include <gtest/gtest.h>
// Needs to be declared after gtest due to g++/gtest
// compilation issues: fixme
import CpmIoController;
#include "Controller/IController.h"
#include "Machine/IMachine.h"
#include "Machine/MachineFactory.h"
#include "nlohmann/json.hpp"

namespace MachEmu::Tests
{
	class MachineTest : public testing::Test
	{
	protected:
		static std::shared_ptr<IController> cpmIoController_;
		static std::shared_ptr<MemoryController> memoryController_;
		static std::shared_ptr<IController> testIoController_;
		static std::unique_ptr<IMachine> machine_;

		static void CheckStatus(uint8_t status, bool zero, bool sign, bool parity, bool auxCarry, bool carry);
		static void LoadAndRun(const char* name, const char* expected);
		static void Run(bool runAsync);
	public:
		static void SetUpTestCase();
		void SetUp();
	};

	std::shared_ptr<IController> MachineTest::cpmIoController_;
	std::shared_ptr<MemoryController> MachineTest::memoryController_;
	std::shared_ptr<IController> MachineTest::testIoController_;
	std::unique_ptr<IMachine> MachineTest::machine_;

	void MachineTest::SetUpTestCase()
	{
		// Note that the tests don't require a json string to be set as it just uses the default values,
		// it is used here for demonstation purposes only
		machine_ = MakeMachine(R"({"cpu":"i8080"})");
		memoryController_ = std::make_shared<MemoryController>();
		cpmIoController_ = std::make_shared<CpmIoController>(static_pointer_cast<IController>(memoryController_));
		testIoController_ = std::make_shared<TestIoController>();
	}

	void MachineTest::SetUp()
	{
		memoryController_->Clear();
		//CP/M Warm Boot is at memory address 0x00, this will be
		//emulated with the exitTest subroutine.
		memoryController_->Load(PROGRAMS_DIR"/exitTest.bin", 0x00);
		//CP/M BDOS print message system call is at memory address 0x05,
		//this will be emulated with the bdosMsg subroutine.
		memoryController_->Load(PROGRAMS_DIR"/bdosMsg.bin", 0x05);
		machine_->SetMemoryController(memoryController_);
		machine_->SetIoController(testIoController_);
		auto err = machine_->SetOptions(R"({"clockResolution":-1,"isrFreq":0,"runAsync":false})");
		EXPECT_EQ(ErrorCode::NoError, err);
	}

	void MachineTest::LoadAndRun(const char* name, const char* expected)
	{
		//EXPECT_NO_THROW
		//(
			machine_->OnSave([expected](std::string&& actual)
			{
				auto actualJson = nlohmann::json::parse(actual);
				auto expectedJson = nlohmann::json::parse(expected);
				EXPECT_TRUE(expectedJson == actualJson["cpu"]);
			});

			std::string dir = PROGRAMS_DIR"/";
			memoryController_->Load((dir + name).c_str(), 0x100);
			machine_->Run(0x100);
		//);
	}

	void MachineTest::CheckStatus(uint8_t status, bool zero, bool sign, bool parity, bool auxCarry, bool carry)
	{
		EXPECT_EQ(carry, (status & 0x01) != 0);
		EXPECT_EQ(parity, (status & 0x04) != 0);
		EXPECT_EQ(auxCarry, (status & 0x10) != 0);
		EXPECT_EQ(zero, (status & 0x40) != 0);
		EXPECT_EQ(sign, (status & 0x80) != 0);
	}
#if 0
	TEST_F(MachineTest, SetNullptrMemoryController)
	{
		EXPECT_ANY_THROW
		(
			//cppcheck-suppress unknownMacro
			machine_->SetMemoryController(nullptr);
		);
	}

	TEST_F(MachineTest, SetNullptrIoController)
	{
		EXPECT_ANY_THROW
		(
			//cppcheck-suppress unknownMacro
			machine_->SetIoController(nullptr);
		);
	}

	TEST_F(MachineTest, SetCpuAfterConstruction)
	{
		EXPECT_ANY_THROW
		(
			//cppcheck-suppress unknownMacro
			machine_->SetOptions(R"({"cpu":"i8080"})");
		);
	}

	TEST_F(MachineTest, NegativeISRFrequency)
	{
		EXPECT_ANY_THROW
		(
			//cppcheck-suppress unknownMacro
			machine_->SetOptions(R"({"isrFreq":-1.0})");
		);
	}

	TEST_F(MachineTest, MethodsThrowAfterRunCalled)
	{
		//cppcheck-suppress unknownMacro
		// Set the resolution so the Run method takes about 1 second to complete therefore allowing subsequent IMachine method calls to throw
		auto err = machine_->SetOptions(R"({"clockResolution":25000000,"runAsync":true})"); // must be async so the Run method returns immediately

		// This is currently not supported on some platforms
		if (err == ErrorCode::NotImplemented)
		{
			return;
		}

		memoryController_->Load(PROGRAMS_DIR"nopStart.bin", 0x00);
		memoryController_->Load(PROGRAMS_DIR"nopEnd.bin", 0xC34F);

		EXPECT_NO_THROW
		(
			machine_->Run();
		);

		EXPECT_ANY_THROW
		(
			machine_->Run(0x100);
		);

		EXPECT_ANY_THROW
		(
			machine_->SetOptions(R"({"isrFreq":1})");
		);

		EXPECT_ANY_THROW
		(
			machine_->SetMemoryController(memoryController_);
		);

		EXPECT_ANY_THROW
		(
			machine_->SetIoController(testIoController_);
		);

		EXPECT_ANY_THROW
		(
			machine_->OnSave([](std::string&&){});
		);

		EXPECT_ANY_THROW
		(
			machine_->Save();
		);

		// Since we are running async we need to wait for completion
		machine_->WaitForCompletion();

		// We are now no longer running, all these methods shouldn't throw

		EXPECT_NO_THROW
		(
			machine_->SetOptions(R"({"isrFreq":1})");
		);

		EXPECT_NO_THROW
		(
			machine_->SetMemoryController(memoryController_);
		);

		EXPECT_NO_THROW
		(
			machine_->SetIoController(testIoController_);
		);

		EXPECT_NO_THROW
		(
			machine_->Save();
		);
	}

	void MachineTest::Run(bool runAsync)
	{
		EXPECT_NO_THROW
		(
			ErrorCode err;

			if (runAsync == true)
			{
				err = machine_->SetOptions(R"({"runAsync":true})");

				// This is currently not supported on some platforms
				if (err == ErrorCode::NotImplemented)
				{
					return;
				}
			}

			// Run a program that should take a second to complete
			// (in actual fact it's 2000047 ticks, 47 ticks over a second.
			// We need to be as close a possible to 2000000 ticks without
			// going under so the cpu sleeps at the end
			// of the program so it maintains sync. It's never going to
			// be perfect, but its close enough for testing purposes).
			memoryController_->Load(PROGRAMS_DIR"nopStart.bin", 0x00);
			memoryController_->Load(PROGRAMS_DIR"nopEnd.bin", 0xC34F);

			// 25 millisecond resolution
			err = machine_->SetOptions(R"({"clockResolution":25000000})");
			EXPECT_EQ(ErrorCode::NoError, err);

			int64_t nanos = 0;

			// If an over sleep occurs after the last batch of instructions are executed during a machine run
			// there is no way to compensate for this which means running a timed test just once will result in
			// sporadic failures. To counter this we will run the machine multiples times and take the average
			// of the accumulated run time, this should smooth out the errors caused by end of program over sleeps.
			int64_t iterations = 1;

			for (int i = 0; i < iterations; i++)
			{
				if (runAsync == true)
				{
					machine_->Run();
					nanos += machine_->WaitForCompletion();
				}
				else
				{
					nanos += machine_->Run();
				}
			}

			auto error = (nanos / iterations) - 1000000000;
			// Allow an average 500 micros of over sleep error
			EXPECT_EQ(true, error >= 0 && error <= 500000);
		);
	}

	TEST_F(MachineTest, RunTimed)
	{
		Run(false);
	}

	TEST_F(MachineTest, RunTimedAsync)
	{
		Run(true);
	}
#endif
	#include "8080Test.cpp"
} // namespace MachEmu::Tests
