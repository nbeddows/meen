module Machine;

namespace Emulator
{
	Machine::Machine()
	{

	}

	void Machine::Run(uint16_t pc)
	{
		cpu_->Reset(pc);
	}

	void Machine::SetMemoryController(std::shared_ptr<IController> controller)
	{
		memoryController_ = controller;
	}

	void Machine::SetIoController(std::shared_ptr<IController> controller)
	{
		ioController_ = controller;
	}
}
