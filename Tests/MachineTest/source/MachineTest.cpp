/*
Copyright (c) 2021-2023 Nicolas Beddows <nicolas.beddows@gmail.com>

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

//#include <future>
#include <gtest/gtest.h>
#include "Controller/IController.h"
#include "Machine/IMachine.h"
#include "Machine/MachineFactory.h"

namespace MachEmu::Tests
{
	class MachineTest : public testing::Test
	{
	protected:
		static std::shared_ptr<MemoryController> memoryController_;
		static std::shared_ptr<IController> ioController_;
		static std::unique_ptr<IMachine> machine_;
	public:
		static void SetUpTestCase();
		void SetUp();
	};

	std::shared_ptr<MemoryController> MachineTest::memoryController_;
	std::shared_ptr<IController> MachineTest::ioController_;
	std::unique_ptr<IMachine> MachineTest::machine_;

	void MachineTest::SetUpTestCase()
	{
		machine_ = MakeMachine();
		memoryController_ = std::make_shared<MemoryController>(16); //16 bit memory bus size
		ioController_ = std::make_shared<TestIoController>();
	}

	void MachineTest::SetUp()
	{
		machine_->SetMemoryController(nullptr);
		machine_->SetIoController(nullptr);
	}

	/*TEST_F(MachineTest, RunNoIoControllerSet)
	{
		//EXPECT_NO_THROW
		//(
			auto path = directory_;
			memoryController_->Load (path /= "cmc.bin", 0x00);
			machine_->SetMemoryController (memoryController_);

			//run the cmc program for one second, we should end in a timeout
			auto future = std::async(std::launch::async, [&]
			{
				machine_->Run();
			});

			EXPECT_EQ(std::future_status::timeout, future.wait_for(seconds(1)));
		//);
	}*/

	TEST_F(MachineTest, RunNoMemoryControllerSet)
	{
		EXPECT_ANY_THROW
		(
			//cppcheck-suppress unknownMacro
			machine_->Run();
		);
	}

	TEST_F(MachineTest, Run)
	{
		EXPECT_NO_THROW
		(
			memoryController_->Load(PROGRAMS_DIR"cmc.bin", 0x00);
			machine_->SetMemoryController(memoryController_);
			machine_->SetIoController(ioController_);
			machine_->Run();
		);
	}
} // namespace MachEmu::Tests
