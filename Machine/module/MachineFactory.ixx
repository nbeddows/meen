export module MachineFactory;

import <memory>;
import IMachine;

namespace Emulator
{
	/** Create a machine.

		This function is the main entry point into machemu.dll.

		@return		std::unique_ptr<IMachine>	An empty machine that can be loaded with memory and io controllers.
	*/
	export DLL_EXP_IMP std::unique_ptr<IMachine> MakeMachine();
}