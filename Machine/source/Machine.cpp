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

#include <cinttypes>
#include <nlohmann/json.hpp>

#include "CpuClock/CpuClockFactory.h"
#include "Cpu/CpuFactory.h"
#include "Machine/Machine.h"
#include "Utils/Utils.h"

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

		return opt_.SetOptions(options);
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
		snprintf(str, 32, "{\"clockResolution\":%" PRIi64 "}", clockResolution);
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
		auto launchPolicy = opt_.RunAsync() ? std::launch::async : std::launch::deferred;

		auto machineLoop = [this]
		{
			auto dataBus = systemBus_.dataBus;
			auto controlBus = systemBus_.controlBus;
			auto currTime = nanoseconds::zero();
			int64_t totalTicks = 0;
			int64_t lastTicks = 0;
			auto loadLaunchPolicy = opt_.LoadAsync() ? std::launch::async : std::launch::deferred;
			auto saveLaunchPolicy = opt_.SaveAsync() ? std::launch::async : std::launch::deferred;
			std::future<std::string> onLoad;
			std::future<void> onSave;

			auto loadMachineState = [this](std::string&& str)
			{
				try
				{
					auto memUuid = memoryController_->Uuid();

					if (memUuid == std::array<uint8_t, 16>{})
					{
						throw std::runtime_error("Invalid memory controller uuid for load interrupt");
					}

					auto json = nlohmann::json::parse(str);

					cpu_->Load(json["cpu"].dump());

					// perform checks to make sure that this machine load state is compatible with this machine

					// The memory controllers must be the same
					auto jsonUuid = Utils::TxtToBin("base64", "none", 16, json["memory"]["uuid"].get<std::string>());

					if (jsonUuid.size() != memUuid.size() || std::equal(jsonUuid.begin(), jsonUuid.end(), memUuid.begin()) == false)
					{
						throw std::runtime_error("Incompatible memory controller");
					}

					std::vector<uint8_t> rom(opt_.RomSize());

					for (auto addr = opt_.RomOffset(); addr < opt_.RomSize(); addr++)
					{
						rom[addr] = memoryController_->Read(addr);
					}

					// The rom must be the same
					auto jsonMd5 = Utils::TxtToBin("base64", "none", 16, json["memory"]["rom"].get<std::string>());
					auto romMd5 = Utils::Md5(rom.data(), rom.size());

					if (jsonMd5.size() != romMd5.size() || std::equal(jsonMd5.begin(), jsonMd5.end(), romMd5.begin()) == false)
					{
						throw std::runtime_error("Incompatible rom");
					}

					// decode and decompress the ram
					auto jsonRam = json["memory"]["ram"];
					auto ram = Utils::TxtToBin(jsonRam["encoder"].get<std::string>(),
						jsonRam["compressor"].get<std::string>(),
						jsonRam["size"].get<uint32_t>(),
						jsonRam["bytes"].get<std::string>());

					// Make sure the ram size matches the layout
					if (ram.size() != opt_.RamSize())
					{
						throw std::runtime_error("Incompatible ram");
					}

					// write it back to memory
					auto addr = opt_.RamOffset();

					for (const auto& r : ram)
					{
						memoryController_->Write(addr++, r);
					}
				}
				catch (const std::exception& e)
				{
					// log the exception - e.what()
					printf("%s\n", e.what());
				}
			};

			auto waitOnHandlers = [&]
			{
				// Make sure that any outstanding onLoad requests have completed
				if (onLoad.valid() == true)
				{
					loadMachineState(onLoad.get());
				}

				// Make sure that any outstanding onSave requests have completed
				if (onSave.valid() == true)
				{
					onSave.get();
				}
			};

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

					switch (isr)
					{
						case ISR::Zero:
						case ISR::One:
						case ISR::Two:
						case ISR::Three:
						case ISR::Four:
						case ISR::Five:
						case ISR::Six:
						case ISR::Seven:
						{
							controlBus->Send(Signal::Interrupt);
							dataBus->Send(static_cast<uint8_t>(isr));
							break;
						}
						case ISR::Load:
						{
							if (onLoad_ != nullptr)
							{
								waitOnHandlers();

								onLoad = std::async(loadLaunchPolicy, [this]
								{
									// Calling out into user land, make sure we don't leak any exceptions
									try
									{
										auto json = onLoad_();

										if (json != nullptr)
										{
											// return a copy of the json c string as a std::string
											return std::string(json);
										}
										else
										{
											throw std::runtime_error("empty json load state");
										}
									}
									catch (const std::exception& e)
									{
										// todo: log the exception to a log file
										printf("%s\n", e.what());
										return std::string("");
									}
								});
							}
							break;
						}
						case ISR::Save:
						{
							// Don't do the save if the onSave method has not been set.
							if (onSave_ != nullptr)
							{
								try
								{
									auto memUuid = memoryController_->Uuid();

									if (memUuid == std::array<uint8_t, 16>{})
									{
										throw std::runtime_error("Invalid memory controller uuid for save interrupt");
									}

									waitOnHandlers();

									auto rm = [this](uint16_t offset, uint16_t size)
									{
										std::vector<uint8_t> mem(size);

										for (auto& byte : mem)
										{
											byte = memoryController_->Read(offset++);
										}

										return mem;
									};

									auto rom = rm(opt_.RomOffset(), opt_.RomSize());
									auto ram = rm(opt_.RamOffset(), opt_.RamSize());
									auto fmtStr = "{\"cpu\":%s,\"memory\":{\"uuid\":\"%s\",\"rom\":\"%s\",\"ram\":{\"encoder\":\"%s\",\"compressor\":\"%s\",\"size\":%d,\"bytes\":\"%s\"}}}";
									auto romMd5 = Utils::Md5(rom.data(), rom.size());

									// todo - replace snprintf with std::format
									auto writeState = [&](size_t& dataSize)
									{
										std::string str;
										char* data = nullptr;

										if (dataSize > 0)
										{
											str.resize(dataSize);
											data = str.data();
										}

										//cppcheck-suppress nullPointer
										dataSize = snprintf(data, dataSize, fmtStr,
											cpu_->Save().c_str(),
											Utils::BinToTxt("base64", "none", memUuid.data(), memUuid.size()).c_str(),
											Utils::BinToTxt("base64", "none", romMd5.data(), romMd5.size()).c_str(),
											opt_.Encoder().c_str(), opt_.Compressor().c_str(), ram.size(),
											Utils::BinToTxt(opt_.Encoder(), opt_.Compressor(), ram.data(), ram.size()).c_str()) + 1;

										return str;
									};

									size_t count = 0;
									writeState(count);

									onSave = std::async(saveLaunchPolicy, [this, state = writeState(count)]
									{
										// Calling out into user land, make sure we don't leak any exceptions
										try
										{
											onSave_(state.c_str());
										}
										catch (const std::exception& e)
										{
											// todo: log the exception to a log file
											printf("%s\n", e.what());
										}
									});
								}
								catch (const std::exception& e)
								{
									// log the exception - e.what()
									printf("%s\n", e.what());
								}
							}
							break;
						}
						case ISR::Quit:
						{
							// Wait for any outstanding load/save requests to complete
							waitOnHandlers();
							controlBus->Send(Signal::PowerOff);
							break;
						}
						case ISR::NoInterrupt:
						{
							// no interrupts pending, do any work that is outstanding

							if (onLoad.valid() == true)
							{
								auto status = onLoad.wait_for(nanoseconds::zero());

								if (status == std::future_status::deferred || status == std::future_status::ready)
								{
									loadMachineState(onLoad.get());
								}
							}

							if (onSave.valid() == true)
							{
								auto status = onSave.wait_for(nanoseconds::zero());

								if (status == std::future_status::deferred || status == std::future_status::ready)
								{
									// todo onSave needs to return an error
									onSave.get();

									// todo - log success/failure
								}
							}

							// could also dump any pending logs to disk here
							// example for onLoad could include:
							// when cpu does not match the load state cpu
							// when the memory controller does not match the load state memory controller
							// when the rom does not match the load state rom
						}
						default:
						{
							//assert(0);
							break;
						}
					}

					lastTicks = totalTicks;
				}
			}

			return currTime.count();
		};

		fut_ = std::async(launchPolicy, [this, ml = std::move(machineLoop)]
		{
			return ml();
		});

		if (launchPolicy == std::launch::deferred)
		{
			totalTime = fut_.get();
			running_ = false;
		}

		return totalTime;
	}

	uint64_t Machine::WaitForCompletion()
	{
		uint64_t totalTime = 0;

		if (fut_.valid() == true)
		{
			totalTime = fut_.get();
			running_ = false;
		}

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

	void Machine::OnSave(std::function<void(const char* json)>&& onSave)
	{
		if (running_ == true)
		{
			throw std::runtime_error("The machine is running");
		}

		onSave_ = std::move(onSave);
	}

	void Machine::OnLoad(std::function<const char*()>&& onLoad)
	{
		if (running_ == true)
		{
			throw std::runtime_error("The machine is running");
		}

		onLoad_ = std::move(onLoad);
	}

	std::string Machine::Save() const
	{
		if (running_ == true)
		{
			throw std::runtime_error("The machine is running");
		}

		if (memoryController_ == nullptr)
		{
			throw std::runtime_error("memory controller not set!");
		}

		auto rm = [this](uint16_t offset, uint16_t size)
		{
			std::vector<uint8_t> mem(size);

			for (auto& byte : mem)
			{
				byte = memoryController_->Read(offset++);
			}

			return mem;
		};

		auto rom = rm(opt_.RomOffset(), opt_.RomSize());
		auto ram = rm(opt_.RamOffset(), opt_.RamSize());
		auto fmtStr = "{\"cpu\":%s,\"memory\":{\"uuid\":\"%s\",\"rom\":\"%s\",\"ram\":{\"encoder\":\"%s\",\"compressor\":\"%s\",\"size\":%d,\"bytes\":\"%s\"}}}";
		auto memUuid = memoryController_->Uuid();
		auto romMd5 = Utils::Md5(rom.data(), rom.size());
		auto writeState = [&](char* data, size_t dataSize)
		{
			auto count = snprintf(data, dataSize, fmtStr,
				cpu_->Save().c_str(),
				Utils::BinToTxt("base64", "none", memUuid.data(), memUuid.size()).c_str(),
				Utils::BinToTxt("base64", "none", romMd5.data(), romMd5.size()).c_str(),
				opt_.Encoder().c_str(), opt_.Compressor().c_str(), ram.size(),
				Utils::BinToTxt(opt_.Encoder(), opt_.Compressor(), ram.data(), ram.size()).c_str());
			return count;
		};

		auto count = writeState(nullptr, 0) + 1;
		std::string state(count, '\0');
		writeState(state.data(), count);
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