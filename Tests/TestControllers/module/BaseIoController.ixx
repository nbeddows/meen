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

export module BaseIoController;

import <chrono>;
import <cstdint>;
import Base;
import IController;

namespace MachEmu
{
    export class BaseIoController : public IController
	{
		private:
			/** powerOff_

				Signals the control bus when the current instruction finishes
				executing that it is time to shutdown.

				The signal is sent during the servicing of interrupts as it
				is guaranteed that no instructions are currently executing
				at that time.
			*/
			//cppcheck-suppress unusedStructMember
			bool powerOff_{};
		protected:
            BaseIoController() = default;
            ~BaseIoController() = default;

			void Write(uint16_t ioDeviceNumber, uint8_t value) override;			
			ISR ServiceInterrupts(std::chrono::nanoseconds currTime, uint64_t cycles) override;
	};
} // namespace MachEmu