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

import SystemBus;

namespace Emulator::Bus::Tests
{
	class SystemBusTest : public testing::Test
	{
	protected:
		//can add other bus types here
		//32bit address, 16bits data, 8 bits control
		//static SystemBus<uint32_t, uint16_t, 8> systemBus_;
		static SystemBus<uint16_t, uint8_t, 8> systemBus_;
	};

	SystemBus<uint16_t, uint8_t, 8> SystemBusTest::systemBus_;

	TEST_F(SystemBusTest, ControlBusClock)
	{
		systemBus_.controlBus->Send(Signal::Clock);
		EXPECT_TRUE(systemBus_.controlBus->Receive(Signal::Clock));
		EXPECT_FALSE(systemBus_.controlBus->Receive(Signal::Clock));
	}

	TEST_F(SystemBusTest, ControlBusInterrupt)
	{
		systemBus_.controlBus->Send(Signal::Interrupt);
		EXPECT_TRUE(systemBus_.controlBus->Receive(Signal::Interrupt));
		EXPECT_FALSE(systemBus_.controlBus->Receive(Signal::Interrupt));
	}

	TEST_F(SystemBusTest, ControlBusPowerOn)
	{
		systemBus_.controlBus->Send(Signal::PowerOn);
		EXPECT_TRUE(systemBus_.controlBus->Receive(Signal::PowerOn));
		EXPECT_FALSE(systemBus_.controlBus->Receive(Signal::PowerOn));
	}

	TEST_F(SystemBusTest, ControlBusPowerOff)
	{
		systemBus_.controlBus->Send(Signal::PowerOff);
		EXPECT_TRUE(systemBus_.controlBus->Receive(Signal::PowerOff));
		EXPECT_FALSE(systemBus_.controlBus->Receive(Signal::PowerOff));
	}

	TEST_F(SystemBusTest, ControlBusMemoryRead)
	{
		systemBus_.controlBus->Send(Signal::MemoryRead);
		EXPECT_TRUE(systemBus_.controlBus->Receive(Signal::MemoryRead));
		EXPECT_FALSE(systemBus_.controlBus->Receive(Signal::MemoryRead));
	}

	TEST_F(SystemBusTest, ControlBusMemoryWrite)
	{
		systemBus_.controlBus->Send(Signal::MemoryWrite);
		EXPECT_TRUE(systemBus_.controlBus->Receive(Signal::MemoryWrite));
		EXPECT_FALSE(systemBus_.controlBus->Receive(Signal::MemoryWrite));
	}

	TEST_F(SystemBusTest, ControlBusIoRead)
	{
		systemBus_.controlBus->Send(Signal::IoRead);
		EXPECT_TRUE(systemBus_.controlBus->Receive(Signal::IoRead));
		EXPECT_FALSE(systemBus_.controlBus->Receive(Signal::IoRead));
	}

	TEST_F(SystemBusTest, ControlBusIoWrite)
	{
		systemBus_.controlBus->Send(Signal::IoWrite);
		EXPECT_TRUE(systemBus_.controlBus->Receive(Signal::IoWrite));
		EXPECT_FALSE(systemBus_.controlBus->Receive(Signal::IoWrite));
	}

	TEST_F(SystemBusTest, DataBus)
	{
		systemBus_.dataBus->Send(0xFF);
		EXPECT_EQ(0xFF, systemBus_.dataBus->Receive());
		EXPECT_EQ(0x00, systemBus_.dataBus->Receive());
	}

	TEST_F(SystemBusTest, AddressBus)
	{
		systemBus_.addressBus->Send(0xABCD);
		EXPECT_EQ(0xABCD, systemBus_.addressBus->Receive());
		EXPECT_EQ(0x0000, systemBus_.addressBus->Receive());
	}
}
