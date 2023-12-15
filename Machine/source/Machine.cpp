/*
Copyright (c) 2021-2023 Nicolas Beddows <nicolas.beddows@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

module Machine;

import <chrono>;
import <functional>;

import Base;
import IController;
import ICpuClock;
import ICpu;
import CpuClockFactory;
import CpuFactory;
import SystemBus;

using namespace std::chrono;

namespace MachEmu
{
	Machine::Machine()
	{
		clock_ = MakeCpuClock(systemBus_.controlBus, nanoseconds(2000));
		cpu_ = Make8080(systemBus_, std::bind(&Machine::ProcessControllers, this, std::placeholders::_1));
	}

	void Machine::ProcessControllers(const SystemBus<uint16_t, uint8_t, 8>&& systemBus)
	{
		auto controlBus = systemBus.controlBus;
		auto addressBus = systemBus.addressBus;
		auto dataBus = systemBus.dataBus;

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

		uint64_t totalCycles = 0;

		while (controlBus->Receive(Signal::PowerOff) == false)
		{
			auto currTime = clock_->Tick();

			//check if we can tick the cpu
			if (controlBus->Receive(Signal::Clock) == true)
			{
				//Execute the next instruction
				auto cycles = cpu_->Execute();

				if (ioController_ != nullptr)
				{
					//Check the IO to see if any interrupts are pending, don't check if we are in the middle of executing an instruction
					//otherwise we will overload the databus.
					if (cycles > 0)
					{
						totalCycles += cycles;
						
						/* TODO

							We should be able to launch ServiceInterrupts on a
							separate task.
						*/

						//auto isr = ioController_->ServiceInterrupts(nanoseconds(tick));
						auto isr = ioController_->ServiceInterrupts(/*currTime,*/ totalCycles);

						if (isr != ISR::NoInterrupt)
						{
							if (isr != ISR::Quit)
							{
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
	}

	void Machine::SetMemoryController(const std::shared_ptr<IController>& controller)
	{
		memoryController_ = controller;
	}

	void Machine::SetIoController(const std::shared_ptr<IController>& controller)
	{
		ioController_ = controller;
	}
} // namespace MachEmu
