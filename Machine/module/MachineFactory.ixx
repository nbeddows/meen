export module MachineFactory;

import <memory>;
import IMachine;

namespace Emulator
{
	export std::unique_ptr<IMachine> MakeMachine();
}