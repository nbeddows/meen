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
#include "Controller/IController.h"

export module BaseIoController;

import <cstdint>;

namespace MachEmu
{
	/** Base IO controller
		
		A minimal base class that contains shared functionality between all
		the test IO Controllers.
	*/
    export class BaseIoController : public IController
	{
		private:
			/** Power off signal

				A signal to indicate to the ServiceInterrupts routine that
				it is time to shutdown.

				@remark		The signal is handled during the servicing of interrupts as it
							is guaranteed that no instructions are currently executing
							at that time.
			*/
			//cppcheck-suppress unusedStructMember
			bool powerOff_{};

			/** Machine save signal
			
				A signal to indicate to the ServiceInterrupts routine that the machine
				should save its current state.

				@remark		The signal is handled during the servicing of interrupts as it
							is guaranteed that no instructions are currently executing
							at that time.
			*/
			//cppcheck-suppress unusedStructMember
			bool save_{};

			/** Machine load signal

				A signal to indicate to the ServiceInterrupts routine that the machine
				should load a new machine state.

				@remark		The signal is handled during the servicing of interrupts as it
							is guaranteed that no instructions are currently executing
							at that time.
			*/
			//cppcheck-suppress unusedStructMember
			bool load_{};

			/** Save on cycle count
			
				The number of cycles processed as seen by the ServiceInterrupts methods before an
				ISR::Save interrupt is generated.
			*/
			//cppcheck-suppress unusedStructMember
			int64_t saveCycleCount_{-1};
		protected:
			/** Base IO controller write
			
				Writes a specifed value to the give port number.

				@param	port	The port to be written to. The power off signal will
								be enabled when the port number is 0xFF.

				@param	value	The value to be written to the specified port.

				@remark			The value parameter is unused.

				@see			powerOff_
			*/
			void Write(uint16_t port, uint8_t value) override;			
			
			/**	Base IO interrupt handler
			
				Checks the powerOff_ signal. When it is true, generate an ISR::Quit interrupt.

				@param	currTime	The time in nanoseconds of the machine clock.

				@param	cycles		The total number of cycles that have elapsed.

				@return				ISR::Quit when the powerOff_ signal is true, ISR::NoInterrupt otherwise.

				@remark				The only way a machine can exit is when an ISR::Quit interrupt is generated.
			*/
			ISR ServiceInterrupts(uint64_t currTime, uint64_t cycles) override;
		public:
			/** Save state after N cycles

				Generate an ISR::Save interrupt from ServiceInterrupts when the Nth cycle has elapsed.

				@param	cycleCount	The number of the cpu cycles to execute before the save interrupt is triggered.
				
				@remark				The cycle count must be one that seen by the ServiceInterrupts method 
									otherwise no interrupt will be generated.
			*/
			void SaveStateOn(int64_t cycleCount);
	};
} // namespace MachEmu