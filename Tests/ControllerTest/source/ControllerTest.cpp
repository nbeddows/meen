#include <gtest/gtest.h>

import Controller;

namespace Emulator::Controller::Tests
{
	class ControllerTest : public testing::Test
	{
	protected:
		static std::unique_ptr<DefaultMemoryController> memoryController_;
	public:
		static void SetUpTestCase();
	};

	std::unique_ptr<DefaultMemoryController> ControllerTest::memoryController_;

	void ControllerTest::SetUpTestCase()
	{
		//16 - size of the address bus in bits
		memoryController_ = std::make_unique<DefaultMemoryController>(16);
	}

	TEST_F(ControllerTest, Load)
	{
		EXPECT_NO_THROW
		(
			//load one byte compliment carry program into memory
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
