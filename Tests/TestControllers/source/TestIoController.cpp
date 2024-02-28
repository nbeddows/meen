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

module;

#include "Base/Base.h"

module TestIoController;

namespace MachEmu
{
    uint8_t TestIoController::Read(uint16_t deviceNumber)
	{
		uint8_t deviceData = 0x00;

		switch (deviceNumber)
		{
			case 0x00:
			{
				//return the data of device 0.
				deviceData = deviceData_;
				break;
			}
			default:
			{
				//we don't support other devices, return 0.
				break;
			}
		}

		return deviceData;
	}

	void TestIoController::Write(uint16_t deviceNumber, uint8_t value)
	{
		switch (deviceNumber)
		{
			case 0:
			{
				//Update device 0 with the contents of the accumulator.
				deviceData_ = value;
				break;
			}
			default:
			{
				BaseIoController::Write (deviceNumber, value);
				break;
			}
		}
	}

	ISR TestIoController::ServiceInterrupts(uint64_t currTime, uint64_t cycles)
	{
		auto isr = BaseIoController::ServiceInterrupts(currTime, cycles);

		if (isr == ISR::NoInterrupt)
		{
			//Fire interrupt rst 1 every second, the cpu will only acknowledge
			//the interrupt if the test programs have interrupts enabled,
			//otherwise it will be ignored.
			auto t = currTime - lastTime_;

			if (t >= 0)
			{
				if (t > 1000000000) // 1 seconds in nanos
				{
					lastTime_ = currTime;
					isr = ISR::One;
				}
			}
			else
			{
				//If the cpu clock gets restarted for example,
				//lastTime will be stale, so we reset it.
				lastTime_ = currTime;
			}
		}

		return isr;
	}
} // namespace MachEmu