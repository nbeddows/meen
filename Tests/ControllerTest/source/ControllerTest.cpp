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


#include <array>
#include <gtest/gtest.h>
#include <memory>

#include "TestControllers/CpmIoController.h"
#include "TestControllers/MemoryController.h"
#include "TestControllers/TestIoController.h"

namespace MachEmu::Tests
{
	class ControllerTest : public testing::Test
	{
	protected:
		static std::unique_ptr<CpmIoController> cpmIoController_;
		static std::unique_ptr<MemoryController> memoryController_;
		static std::unique_ptr<TestIoController> testIoController_;
	public:
		static void SetUpTestCase();
	};

	std::unique_ptr<CpmIoController> ControllerTest::cpmIoController_;
	std::unique_ptr<MemoryController> ControllerTest::memoryController_;
	std::unique_ptr<TestIoController> ControllerTest::testIoController_;

	void ControllerTest::SetUpTestCase()
	{
		//16 - size of the address bus in bits
		memoryController_ = std::make_unique<MemoryController>();
		cpmIoController_ = std::make_unique<CpmIoController>(nullptr);
		testIoController_ = std::make_unique<TestIoController>();
	}

	TEST_F(ControllerTest, Uuids)
	{
		auto mcUuid = std::array<uint8_t, 16>{ 0xCD, 0x18, 0xD8, 0x67, 0xDD, 0xBF, 0x4D, 0xAA, 0xAD, 0x5A, 0xBA, 0x1C, 0xEB, 0xAE, 0xB0, 0x31 };
		auto tcUuid = std::array<uint8_t, 16>{ 0xD8, 0x62, 0xFA, 0xBD, 0xDE, 0xDD, 0x47, 0xB7, 0x8C, 0x38, 0xD0, 0xDE, 0xB5, 0xCC, 0x45, 0xBE };
		auto cpmcUuid = std::array<uint8_t, 16>{ 0x32, 0x8C, 0xCF, 0x78, 0x76, 0x1B, 0x48, 0xA4, 0x98, 0x2C, 0x1A, 0xAA, 0x5F, 0x14, 0x31, 0x24 };

		EXPECT_EQ(mcUuid, memoryController_->Uuid());
		EXPECT_EQ(tcUuid, testIoController_->Uuid());
		EXPECT_EQ(cpmcUuid, cpmIoController_->Uuid());
	}

	TEST_F(ControllerTest, Load)
	{
		EXPECT_NO_THROW
		(
			//load one byte compliment carry program into memory
			//cppcheck-suppress unknownMacro
			memoryController_->Load (PROGRAMS_DIR"cmc.bin", 0);
		);
	}

	TEST_F(ControllerTest, LoadThrowInvalidOffset)
	{
		EXPECT_ANY_THROW
		(
			//load three byte compliment accumulator program starting at the end of memory, this should throw
			memoryController_->Load(PROGRAMS_DIR"cma.bin", static_cast<uint16_t>(memoryController_->Size() - 1));
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
			memoryController_->Load(PROGRAMS_DIR"cmc.bin", 0x00);
			value = memoryController_->Read(0x00);
		);

		EXPECT_EQ(0x3F, value);
	}
} // namespace MachEmu::Tests
