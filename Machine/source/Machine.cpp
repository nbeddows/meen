module;

#include "SDL.h"

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

	void Machine::RunCpu()
	{
		auto totalCycles = 0;
		auto addressBus = systemBus_.addressBus;
		auto dataBus = systemBus_.dataBus;
		auto controlBus = systemBus_.controlBus;

		while (totalCycles < 16666)
		{
			auto cycles = cpu_->Execute();

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
			}

			totalCycles += cycles;
		}
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
		
#if 0		
		uint32_t last_tic = SDL_GetTicks();  // milliseconds

		//int exitCount = 0;
		
		while (controlBus->Receive(Signal::PowerOff) == false)
		{
			if ((SDL_GetTicks() - last_tic) >= 1000.0/60.0)
			{
				last_tic = SDL_GetTicks();
				
				RunCpu();

				controlBus->Send(Signal::Interrupt);
				dataBus->Send(static_cast<uint8_t>(ISR::One));

				RunCpu();

				ioController_->ServiceInterrupts (nanoseconds::zero(), 0);

				controlBus->Send(Signal::Interrupt);
				dataBus->Send(static_cast<uint8_t>(ISR::Two));

				//exitCount++;

				//if (exitCount == 1000)
				//{
				//	controlBus->Send(Signal::PowerOff);
				//	continue;
				//}

				//if (SDL_GetTicks() - last_tic > 1000.0 / 60.0)
				//{
				//	printf ("TOO SLOW: %d\n", SDL_GetTicks() - last_tic);
				//}
				//else
				//{
				//	printf("GOOD: %d\n", SDL_GetTicks() - last_tic);
				//}
			}
		}
#else		
		uint64_t totalCycles = 0;

		while (controlBus->Receive(Signal::PowerOff) == false)
		{
			auto currTime = clock_->Tick();

			//check if we can tick the cpu
			if (controlBus->Receive(Signal::Clock) == true)
			{
				//(continue to) execute the next instruction
				auto cycles = cpu_->Execute();

				//tick++;

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
					if (cycles > 0)
					{
						totalCycles += cycles;
						
						//if (totalCycles >= 16666)
						//{
						//	printf ("TotalCycles: %I64d\n", totalCycles);
						//	totalCycles = 0;
						//}
						
						/* TODO

							We should be able to launch ServiceInterrupts on a
							separate task.
						*/

						//auto isr = ioController_->ServiceInterrupts(nanoseconds(tick));
						auto isr = ioController_->ServiceInterrupts(currTime, totalCycles);

						if (isr != ISR::NoInterrupt)
						{
							if (isr != ISR::Quit)
							{
								//printf ("Ticks: %d\n", tick);
								//tick = 0;
								
								controlBus->Send(Signal::Interrupt);
								dataBus->Send(static_cast<uint8_t>(isr));
							}
							else
							{
								controlBus->Send(Signal::PowerOff);
							}
						}
					}
				}
			}
		}
#endif
	}

	void Machine::SetMemoryController(const std::shared_ptr<IController>& controller)
	{
		memoryController_ = controller;
	}

	void Machine::SetIoController(const std::shared_ptr<IController>& controller)
	{
		ioController_ = controller;
	}
}
