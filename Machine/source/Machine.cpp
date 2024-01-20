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
module;

#include "Base/Base.h"
#include "Controller/IController.h"

module Machine;

import <array>;
import <chrono>;
import <functional>;

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
		clock_ = MakeCpuClock(2000000);
		cpu_ = Make8080(systemBus_, std::bind(&Machine::ProcessControllers, this, std::placeholders::_1));
	}

	ErrorCode Machine::SetClockResolution(int64_t clockResolution)
	{
		return clock_->SetTickResolution(nanoseconds(clockResolution));
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

	uint64_t Machine::Run(uint16_t pc)
	{		
		if (memoryController_ == nullptr)
		{
			throw std::runtime_error ("No memory controller has been set");
		}

		if (ioController_ == nullptr)
		{
			throw std::runtime_error("No io controller has been set");
		}

		auto dataBus = systemBus_.dataBus;
		auto controlBus = systemBus_.controlBus;
		auto currTime = nanoseconds::zero();

		cpu_->Reset(pc);
		clock_->Reset();

		uint64_t totalCycles = 0;

		while (controlBus->Receive(Signal::PowerOff) == false)
		{
			//Execute the next instruction
			auto cycles = cpu_->Execute();
			currTime = clock_->Tick(cycles);
			totalCycles += cycles;

			if (ioController_ != nullptr)
			{
				auto isr = ioController_->ServiceInterrupts(currTime.count(), totalCycles);

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

		return currTime.count();
	}

	void Machine::SetMemoryController(const std::shared_ptr<IController>& controller)
	{
		if (controller == nullptr)
		{
			throw std::invalid_argument("Argument 'controller' can not be nullptr");
		}
		
		memoryController_ = controller;
	}

	void Machine::SetIoController(const std::shared_ptr<IController>& controller)
	{
		if (controller == nullptr)
		{
			throw std::invalid_argument("Argument 'controller' can not be nullptr");
		}

		ioController_ = controller;
	}

	std::vector<uint8_t> Machine::GetState() const
	{
		return cpu_->GetState();
	}
} // namespace MachEmu
