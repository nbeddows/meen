/*
Copyright (c) 2021-2024 Nicolas Beddows <nicolas.beddows@gmail.com>

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

#include <cstring>
//#include <format>

#include "Base/Base.h"
#include "Controller/IController.h"

module Machine;

import <chrono>;
import <functional>;
#ifdef _WINDOWS
import <future>;
#endif
import <memory>;
import <string_view>;

import ICpuClock;
import ICpu;
import CpuClockFactory;
import CpuFactory;
import SystemBus;

using namespace std::chrono;

namespace MachEmu
{
	Machine::Machine(const char* options)
	{
		auto err = SetOptions(options);

		if (err != ErrorCode::NoError)
		{
			throw std::invalid_argument("All options must be valid, check your configuration!");
		}

		// if no cpu type specified, set the default
		if(opt_.CpuType().empty() == true)
		{
			SetOptions(R"({"cpu":"i8080"})");
		}

		auto cpuType = opt_.CpuType().c_str();

		if (strncmp(cpuType, "i8080", strlen(cpuType)) == 0)
		{
			clock_ = MakeCpuClock(2000000);
			cpu_ = Make8080(systemBus_, std::bind(&Machine::ProcessControllers, this, std::placeholders::_1));
		}
		else
		{
			throw std::invalid_argument("Unsupported cpu type");
		}
	}

	ErrorCode Machine::SetOptions(const char* options)
	{
		if (running_ == true)
		{
			throw std::runtime_error("The machine is running");
		}

		ErrorCode err;

		if (options == nullptr)
		{
			// set all options to their default values
			err = opt_.SetOptions(R"({"clockResolution::-1,"isrFreq":0,"runAsync":false})");
		}
		else
		{
			err = opt_.SetOptions(options);
		}

		return err;
	}

	ErrorCode Machine::SetClockResolution(int64_t clockResolution)
	{
		if (running_ == true)
		{
			throw std::runtime_error("The machine is running");
		}

		if (clockResolution < -1 || clockResolution > 10000000000)
		{
			throw std::runtime_error("Clock resolution out of range");
		}

		char str[32]{};
		snprintf(str, 32, R"({"clockResolution":%lld})", clockResolution);
		opt_.SetOptions(str);
		//opt_.SetOptions(std::format(R"({{"clockResolution":{}}})", clockResolution).c_str());

		int64_t resInTicks = 0;

		auto err = clock_->SetTickResolution(nanoseconds(clockResolution), &resInTicks);

		if (err == ErrorCode::NoError)
		{
			ticksPerIsr_ = opt_.ISRFreq() * resInTicks;
		}

		return err;
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

		if (running_ == true)
		{
			throw std::runtime_error("The machine is running");
		}

		cpu_->Reset(pc);
		clock_->Reset();
		SetClockResolution(opt_.ClockResolution());
		running_ = true;
		uint64_t totalTime = 0;

		auto machineLoop = [this]()
		{
			auto dataBus = systemBus_.dataBus;
			auto controlBus = systemBus_.controlBus;
			auto currTime = nanoseconds::zero();
			int64_t totalTicks = 0;
			int64_t lastTicks = 0;

			while (controlBus->Receive(Signal::PowerOff) == false)
			{
				//Execute the next instruction
				auto ticks = cpu_->Execute();
				currTime = clock_->Tick(ticks);
				totalTicks += ticks;

				// Check if it is time to service interrupts
				if (totalTicks - lastTicks >= ticksPerIsr_)
				{
					auto isr = ioController_->ServiceInterrupts(currTime.count(), totalTicks);

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

					lastTicks = totalTicks;
				}
			}

			return currTime.count();
		};

#ifdef _WINDOWS
		if (opt_.RunAsync() == true)
		{
			fut_ = std::async(std::launch::async, [this, ml = std::move(machineLoop)]()
			{
				return ml();
			});
		}
		else
#endif
		{
			totalTime = machineLoop();
			running_ = false;
		}

		return totalTime;
	}

	uint64_t Machine::WaitForCompletion()
	{
		uint64_t totalTime = 0;

#ifdef _WINDOWS
		if (running_ == true && opt_.RunAsync() == true)
		{
			fut_.wait();
			running_ = false;
			totalTime = fut_.get();
		}
#endif

		return totalTime;
	}

	void Machine::SetMemoryController(const std::shared_ptr<IController>& controller)
	{
		if (controller == nullptr)
		{
			throw std::invalid_argument("Argument 'controller' can not be nullptr");
		}

		if (running_ == true)
		{
			throw std::runtime_error("The machine is running");
		}

		memoryController_ = controller;
	}

	void Machine::SetIoController(const std::shared_ptr<IController>& controller)
	{
		if (controller == nullptr)
		{
			throw std::invalid_argument("Argument 'controller' can not be nullptr");
		}

		if (running_ == true)
		{
			throw std::runtime_error("The machine is running");
		}

		ioController_ = controller;
	}

	std::string Machine::Save() const
	{
		if (running_ == true)
		{
			throw std::runtime_error("The machine is running");
		}

		std::string state{ "{" };
		state += "\"cpu\":"; 
		state += cpu_->Save();
		state += "}";
		return state;
	}

	std::unique_ptr<uint8_t[]> Machine::GetState(int* size) const
	{
		if (running_ == true)
		{
			throw std::runtime_error("The machine is running");
		}

		return cpu_->GetState(size);
	}
} // namespace MachEmu
