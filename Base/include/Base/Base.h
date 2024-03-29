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

#ifndef BASE_H
#define BASE_H

namespace MachEmu
{
	/** Interrupt service routine

		A pre defined set of interrupts that can be returned from IController::ServiceInterrupts.

		@see	IController.
	*/
	enum class ISR
	{
		Zero,					/**< Interrupt 0. */
		One,					/**< Interrupt 1. */
		Two,					/**< Interrupt 2. */
		Three,					/**< Interrupt 3. */
		Four,					/**< Interrupt 4. */
		Five,					/**< Interrupt 5. */
		Six,					/**< Interrupt 6. */
		Seven,					/**< Interrupt 7. */
		Save = 0xFFCF,			/**< Save the machine state. */
		Quit = 0xFFEF,			/**< Exit the IMachine::Run control loop. */
		NoInterrupt,			/**< No interrupt has occurred. */
	};

	/** MachuEmu error codes
	
		@todo		Convert to ErrorCode to std::error_code
	*/
	enum class ErrorCode
	{
		NoError,				/**< Success, no error has occured */
		ClockResolution,		/**< Warning, clock resolution may be inaccurate */
		NotImplemented,			/**< Warning, specifed configuration option is not implemented */
		UnknownOption			/**< Warning, unrecognised configuration option found */
	};
} // namespace MachEmu

#endif // BASE_H