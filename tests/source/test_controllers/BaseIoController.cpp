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


#include "meen/Base.h"
#include "test_controllers/BaseIoController.h"

namespace meen
{
	void BaseIoController::SaveStateOn(int64_t cycleCount)
	{
		saveCycleCount_ = cycleCount;
	}

    void BaseIoController::Write(uint16_t port, [[maybe_unused]] uint8_t value)
	{
		powerOff_ = port == 0xFF;
		save_ = port == 0xFE;
		load_ = port == 0xFD;
	}

	ISR BaseIoController::ServiceInterrupts([[maybe_unused]] uint64_t currTime, [[maybe_unused]] uint64_t cycles)
	{
		auto isr = ISR::NoInterrupt;

		if (powerOff_ == true)
		{
			isr = ISR::Quit;
			powerOff_ = false;
		}
		else if (save_ == true || saveCycleCount_ == cycles)
		{
			isr = ISR::Save;
			save_ = false;
		}
		else if (load_ == true)
		{
			isr = ISR::Load;
			load_ = false;
		}
		
		return isr;
	}
} // namespace meen