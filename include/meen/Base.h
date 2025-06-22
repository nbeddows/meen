/*
Copyright (c) 2021-2025 Nicolas Beddows <nicolas.beddows@gmail.com>

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

namespace meen
{
	/** Interrupt service routines

		A pre defined set of interrupts that can be returned from IController::GenerateInterrupt.

		@remark		Interrupts `Zero` to `Seven` interrupt the cpu and jump to the relevant
					address dictated by that interrupt to execute specific instructions
					before restoring the cpu's pre interrupt state.
		@remark		Interrupts `Save` to `Quit` interrupt the machine to perform the specific
					task as dictated by the interrupt.

		@see		IController
	*/
	enum class ISR
	{
		// Cpu level interrupts
		Zero,					/**< Interrupt 0 */
		One,					/**< Interrupt 1 */
		Two,					/**< Interrupt 2 */
		Three,					/**< Interrupt 3 */
		Four,					/**< Interrupt 4 */
		Five,					/**< Interrupt 5 */
		Six,					/**< Interrupt 6 */
		Seven,					/**< Interrupt 7 */
		// Machine level interrupts
		Save = 0xFFCF,			/**< Save the machine state */
		Load,					/**< Load the machine state */
		Quit,					/**< Exit the IMachine::Run control loop */
		NoInterrupt,			/**< No interrupt has occurred */
	};
} // namespace meen

#endif // BASE_H