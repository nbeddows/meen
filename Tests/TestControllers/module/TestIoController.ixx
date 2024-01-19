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

module;

#include "Base/Base.h"

export module TestIoController;

import <cstdint>;
import BaseIoController;

namespace MachEmu
{
    /** TestIoController

		A minimal implementaion for an io controller. It allows
		reading and writing to one device (device0). ISR:One
		interrupt is triggered every second.

		@remark		This IO controller is purely academic, it's
					main use is for unit tests, in reality the
					device data would be an interface into actual
					IO, for example, a keyboard or a mouse.
	*/
	export class TestIoController final : public BaseIoController
	{
	private:
		/** lastTime_
		
			Track the cpu time so we can trigger interrupts
			at one second intervals.
		*/
		uint64_t lastTime_;

		/** deviceData_

			The io data to be maintained by this 'io device'.

			@remark		The mock device will be initialised to an
						arbitary value, this can be useful, for
						example, during tests.
		*/
		//cppcheck-suppress unusedStructMember
		uint8_t deviceData_{ 0xAA };
	public:
		/**	Read from a device

			Read the contents of the specifed io device.

			@param	ioDeviceNumber	The io device number to read from.
									Only one device is supported, ie;
									device number 0.

			@return					The contents of the io device at the
									time of the function call.

			@see					IController::Read()
		*/
		uint8_t Read(uint16_t ioDeviceNumber) final;

		/** Write to a device

			Write the specified value to the specified io device.

			@param	ioDeviceNumber	The io device number to write to.
									Only one device is supported, ie;
									device number 0.

			@param	value			The data to write to the io device.

			@see					IController::Write()
		*/
		void Write(uint16_t ioDeviceNumber, uint8_t value) final;

		/** Interrupt handler

			Checks the io controller to see if any interrupts are pending.
			In this example controller, ISR::One will be triggered at
			one second intervals.

			@param	currTime	The time in nanoseconds of the machine clock.

			@param	cycles		The total number of cycles that have elapsed.

			@return				The interrupt that requires servicing by the
								cpu. In this case, ISR::One every second.

			@see IContoller::ServiceInterrupts()
		*/
		ISR ServiceInterrupts(uint64_t currTime, uint64_t cycles) final;
	};
} // namespace MachEmu