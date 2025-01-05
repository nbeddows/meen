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

#ifndef CPMIOCONTROLLER_H
#define CPMIOCONTROLLER_H

#include <array>
#include <memory>
#include <string>

#include "test_controllers/BaseIoController.h"

namespace meen
{
	/** Basic CP/M IO Controller

		A minimal IO controller which emulates 8 bit CP/M BDOS
		console output and output string system calls.
	*/
	class CpmIoController final : public BaseIoController
	{
	private:
		/** CP/M IO Port

			Ports for printing characters.
		*/
		enum class Port
		{
			PrintMode,	//!< 2 - Output a single character into the output buffer. 9 - Output characters into the output buffer from memory until a '$' character is read.
			AddrHi,		//!< The high 8 bit memory address to use when 9 is written to Port::PrintMode.
			Process		//!< Process output message according to the value that was written to Port::PrintMode.
		};
		
		/** Output message buffer

			A character buffer where program output messages are printed to.
		*/
		//cppcheck-suppress unusedStructMember
		std::string message_;

		/** Printing mode

			The BDOS output mode.

			@remark		A value of 9 will output characters from memory
						into the message buffer until a '$' character is read.
						A value of 2 will output a single character into
						the output message buffer.
		*/
		//cppcheck-suppress unusedStructMember
		uint8_t printMode_{};

		/** High memory address

			The high 8 bits of the starting 16 bit memory address
			from which characters will be read from the memory controller
			into the output buffer.
			
			@remark		This value is only used when the print mode is 9.
		*/
		//cppcheck-suppress unusedStructMember
		uint8_t addrHi_{};
	public:
		/** Output message buffer

			The output message generated from one of the print modes.
			
			@return		A copy of the internal message buffer as a std::string.

			@remark		When this method returns the internal message buffer will be cleared.

			@see		CpmIoController::message_
		*/
		std::string Message();

		/**	Uuid

			Unique universal identifier for this controller.

			@return					The uuid as a 16 byte array.
		*/
		std::array<uint8_t, 16> Uuid() const final;

		/** CP/M IO controller read

			@param	port		The port number to read from.
			@param	controller	Unused in this implementation.	

			@return				A uint16_t value specific to the port number.
	
			@remark				Not used by this controller, always returns 0.
		*/
		uint8_t Read(uint16_t port, IController* controller) final;

		/** CP/M IO controller write

			Writes the specified value to the given port.

			@param	port		The port to be written to.
			@param	value		The value to be written to the specified port when Port is PrintMode or AddrHi.
			@param	controller	The memory controller for the program from which any output message will be read from.

			@remark				When the Port is Port::Process the value is either the low 8 bit address when the Port::PrintMode is 9 or the actual
								value to print when Port::PrintMode is 2.

			@see				CpmIoController::Port
		*/
		void Write(uint16_t port, uint8_t value, IController* controller) final;

		/** CP/M IO interrupt handler

			Checks the IO controller to see if any interrupts are pending.

			@param	currTime	The time in nanoseconds of the machine clock.
			@param	cycles		The total number of cycles that have elapsed.
			@param	controller	Unused by this implementation.
			
			@return				The interrupt that requires servicing by the
								cpu.

			@remark				Currently, no interrupts are triggered by this
								io controller. The return value will always
								be zero.

			@see				IContoller::ServiceInterrupts
		*/
		ISR ServiceInterrupts(uint64_t currTime, uint64_t cycles, IController* controller) final;
	};
} // namespace meen

#endif // CPMIOCONTROLLER_H