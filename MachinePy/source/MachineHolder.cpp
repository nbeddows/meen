#include "MachinePy/MachineHolder.h"

namespace MachEmu
{
	MachineHolder::MachineHolder()
	{
		machine_ = MachEmu::MakeMachine();
	}

	MachineHolder::MachineHolder(const char* json)
	{
		machine_ = MachEmu::MakeMachine(json);
	}

	ErrorCode MachineHolder::SetClockResolution(int64_t clockResolution)
	{
		return machine_->SetClockResolution(clockResolution);
	}

	uint64_t MachineHolder::Run(uint16_t offset)
	{
		return machine_->Run(offset);
	}

	void MachineHolder::SetIoController(MachEmu::IController* controller)
	{
		// custom deleter, don't delete this pointer from c++, python owns it
		machine_->SetIoController(std::shared_ptr<MachEmu::IController>(controller, [](MachEmu::IController*) {}));
	}

	void MachineHolder::SetMemoryController(MachEmu::IController* controller)
	{
		machine_->SetMemoryController(std::shared_ptr<MachEmu::IController>(controller, [](MachEmu::IController*) {}));
	}

	ErrorCode MachineHolder::SetOptions(const char* options)
	{
		return machine_->SetOptions(options);
	}
} // namespace MachEmu
