module Machine;

import <chrono>;

using namespace std::chrono;

namespace Emulator
{
	Machine::Machine()
	{
		clock_ = MakeCpuClock(systemBus_.controlBus, nanoseconds(2000));
		cpu_ = Make8080(systemBus_);
	}

	void Machine::Run(uint16_t pc)
	{		
		if (memoryController_ == nullptr)
		{
			throw std::runtime_error ("No memory controller has been set");
		}
		
		auto addressBus = systemBus_.addressBus;
		auto dataBus = systemBus_.dataBus;
		auto controlBus = systemBus_.controlBus;

		cpu_->Reset(pc);

		while (controlBus->Receive(Signal::PowerOff) == false)
		{
			auto currTime = clock_->Tick();

			//check if we can tick the cpu
			if (controlBus->Receive(Signal::Clock) == true)
			{
				//(continue to) execute the next instruction
				auto executionComplete = cpu_->Execute();

				if (memoryController_ != nullptr)
				{
					//check the control bus to see if there are any operations pending
					if (controlBus->Receive(Signal::MemoryRead))
					{
						dataBus->Send(memoryController_->Read(addressBus->Receive()));
					}

					if (controlBus->Receive(Signal::MemoryWrite))
					{
						memoryController_->Write(addressBus->Receive(), dataBus->Receive());
					}
				}

				if (ioController_ != nullptr)
				{
					if (controlBus->Receive(Signal::IoRead))
					{
						dataBus->Send(ioController_->Read(addressBus->Receive()));
					}

					if (controlBus->Receive(Signal::IoWrite))
					{
						ioController_->Write(addressBus->Receive(), dataBus->Receive());
					}

					//Check the IO to see if any interrupts are pending, don't check if we are in the middle of executing an instruction
					//otherwise we will overload the databus.
					if (executionComplete == true)
					{
						auto isr = ioController_->ServiceInterrupts(currTime);

						if (isr != ISR::NoInterrupt)
						{
							controlBus->Send(Signal::Interrupt);
							dataBus->Send(static_cast<uint8_t>(isr));
						}
					}
				}
			}
		}
	}

	void Machine::SetMemoryController(std::shared_ptr<IController> controller)
	{
		memoryController_ = controller;
	}

	void Machine::SetIoController(std::shared_ptr<IController> controller)
	{
		ioController_ = controller;
	}

	const std::shared_ptr<ControlBus<8>>& Machine::ControlBus() const
	{
		return systemBus_.controlBus;
	}
}
