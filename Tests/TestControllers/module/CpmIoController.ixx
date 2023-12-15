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

export module CpmIoController;

import <memory>;
import <string>;
import Base;
import BaseIoController;

namespace MachEmu
{

	/** CpmIoController

	A basic io controller which emulates 8 bit CP/M BDOS
	console output and output string system calls.

	@discussion		This io controller is used for running the cpudiag tests
					which were originally written targeting CP/M BDOS.
					BDOS calls start from 0x0005 and since we aren't running
					CP/M we can't call 0x0005 to make those system calls, so
					the cpudiag tests program was modified to perform the
					BDOS call through a custom IO, hence eliminating the
					dependence on CP/M BDOS. See cpudiag_readme.txt for
					more information.
	*/
	export class CpmIoController final : public BaseIoController
	{
	private:
		enum class Port
		{
			PrintMode,
			AddrHi,
			Process
		};
		
		/** message_

			The final output buffer that will be used to
			print to.
		*/
		//cppcheck-suppress unusedStructMember
		std::string message_;

		/** memoryController_

			The memory that the message will be read from.
		*/
		std::shared_ptr<IController> memoryController_;

		/** printMode_

			The BDOS output mode.

			A value of 9 will output characters from memory
			into the message buffer until a '$' character is read.
			A value of 2 will output a single character into
			the output message buffer.
		*/
		//cppcheck-suppress unusedStructMember
		uint8_t printMode_{};

		/** addrHi_

			The high 8 bits of the starting 16 bit memory address
			from which characters will be read into the output buffer.
			This value is only used when the print mode is 9.
		*/
		//cppcheck-suppress unusedStructMember
		uint8_t addrHi_{};
	public:
		/** CpmIoController

			An io controller that emulates the CP/M BDOS output routine.
		*/
		explicit CpmIoController(const std::shared_ptr<IController>& memoryController);

		/** Message

			Returns the message buffer. The message buffer is then cleared.

			@return		string	The message buffer.
		*/
		std::string Message();

		/** Read

			Not used by this controller.

			@return		uint8_t		Always zero.
		*/
		uint8_t Read(uint16_t ioDeviceNumber) final;

		/** Write

			Writes the specified value to given port.

			@param	Port			Port::PrintMode -	The character output mode to use when Port::Process is written to.
														9 - Output characters into the output buffer from
															memory until a '$' character is read.
														2 - Output a single charater into the output buffer.
									Port::AddrHi	-	The high 8 bit memory address to use when 9 is written to Port::PrintMode.
									Port::Process	-	Process output message according to the value that was written to Port::PrintMode.
			@param	value			The value to be written to the specified port when Port is PrintMode or AddrHi. When the Port is
									Port::Process the value is either the low 8 bit address when the Port::PrintMode is 9 or the actual
									value to print when Port::PrintMode is 2.
		*/
		void Write(uint16_t Port, uint8_t value) final;

		/** ServiceInterrupts

			Checks the io controller to see if any interrupts are pending.

			@param	currTime	The time in nanoseconds of the cpu clock.

			@return	uint8_t		The interrupt that requires servicing by the
								cpu.

			@discussion			Currently, no interrupts are triggered by this
								io controller. The return value will always
								be zero.

			@see IContoller::ServiceInterrupts()
		*/
		ISR ServiceInterrupts(uint64_t cycles) final;
	};
} // namespace MachEmu