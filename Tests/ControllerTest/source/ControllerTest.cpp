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

#include <gtest/gtest.h>

import ControllerFactory;
import IMemoryController;

namespace Emulator::Controller::Tests
{
	class ControllerTest : public testing::Test
	{
	protected:
		static std::unique_ptr<IMemoryController> memoryController_;
	public:
		static void SetUpTestCase();
	};

	std::unique_ptr<IMemoryController> ControllerTest::memoryController_;

	void ControllerTest::SetUpTestCase()
	{
		//16 - size of the address bus in bits
		memoryController_ = MakeMemoryController(16);
	}

	TEST_F(ControllerTest, Load)
	{
		EXPECT_NO_THROW
		(
			//load one byte compliment carry program into memory
			//cppcheck-suppress unknownMacro
			memoryController_->Load ("../../Programs/cmc.bin", 0);
		);
	}

	TEST_F(ControllerTest, LoadThrowInvalidOffset)
	{
		EXPECT_ANY_THROW
		(
			//load three byte compliment accumulator program starting at the end of memory, this should throw
			memoryController_->Load("../../Programs/cma.bin", static_cast<uint16_t>(memoryController_->Size() - 1));
		);
	}

	TEST_F(ControllerTest, Write)
	{
		uint8_t value = 0;

		EXPECT_NO_THROW
		(
			memoryController_->Write(0x100, 0xFF);
			value = memoryController_->Read(0x100);
		);

		EXPECT_EQ(0xFF, value);
	}

	TEST_F(ControllerTest, Read)
	{
		uint8_t value = 0;

		EXPECT_NO_THROW
		(
			memoryController_->Load("../../Programs/cmc.bin", 0x00);
			value = memoryController_->Read(0x00);
		);

		EXPECT_EQ(0x3F, value);
	}
}
