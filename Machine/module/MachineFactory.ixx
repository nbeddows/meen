export module MachineFactory;

import <memory>;
import IMachine;

namespace Emulator
{
	export DLL_EXP_IMP std::unique_ptr<IMachine> MakeMachine();
}