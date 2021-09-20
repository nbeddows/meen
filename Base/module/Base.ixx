export module Base;

namespace Emulator
{
	/** Interrupt service routine.

		A pre defined set of interrupts that can be returned from IController::ServiceInterrupts.

		@see	IController.
	*/
	export enum class ISR
	{
		Zero,
		One,
		Two,
		Three,
		Four,
		Five,
		Six,
		Seven,
		Quit = 0xEF,	//<	Returned from IController::ServiceInterrupts to exit the IMachine::Run control loop.
		NoInterrupt,
	};
}