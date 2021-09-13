module MachineFactory;

import <memory>;
import Machine;

namespace Emulator
{
	std::unique_ptr<IMachine> MakeMachine()
	{
		return std::make_unique<Machine>();
	}
}
