export module Base;

namespace Emulator
{
	//Interrupt service routine, naming this 'Interrupt' yeilds an internal compiler error!!!
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
		Quit = 0xEF,
		NoInterrupt,
	};
}