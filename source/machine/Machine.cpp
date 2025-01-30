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

#include <assert.h>
#include <bit>
#include <cinttypes>
#include <stdio.h>
#ifdef ENABLE_MEEN_RP2040
#include <pico/multicore.h>
#endif // ENABLE_MEEN_RP2040
#ifdef ENABLE_MEEN_SAVE
#ifdef ENABLE_NLOHMANN_JSON
#include <nlohmann/json.hpp>
#else
#include <ArduinoJson.h>
#endif // ENABLE_NLOHMANN_JSON
#include "meen/utils/Utils.h"
#endif // ENABLE_MEEN_SAVE
#include "meen/clock/CpuClockFactory.h"
#include "meen/cpu/CpuFactory.h"
#include "meen/machine/Machine.h"
#include "meen/utils/ErrorCode.h"

using namespace std::chrono;

namespace meen
{
	Machine::Machine(Cpu cpu)
	{
		switch (cpu)
		{
			case Cpu::i8080:
			{
				clock_ = MakeCpuClock(2000000);
				cpu_ = Make8080();
			}
			default:
			{
				break;
			}
		}
	}

	std::error_code Machine::SetOptions(const char* options)
	{
		if (running_ == true)
		{
			return make_error_code(errc::busy);
		}

		return opt_.SetOptions(options);
	}

	void RunMachine(meen::Machine* m)
	{
		auto currTime = nanoseconds::zero();
		int64_t totalTicks = 0;
		int64_t lastTicks = 0;
		bool quit = false;
		auto cpu_ = m->cpu_.get();
		auto clock_ = m->clock_.get();
		auto ioController = m->ioController_.get();
		auto ticksPerIsr_ = m->ticksPerIsr_;
#ifdef ENABLE_MEEN_SAVE
		auto memoryController = m->memoryController_.get();
		auto opt_ = m->opt_;
		auto onLoad_ = m->onLoad_;
		auto onSave_ = m->onSave_;
		auto loadLaunchPolicy = opt_.LoadAsync() ? std::launch::async : std::launch::deferred;
		auto saveLaunchPolicy = opt_.SaveAsync() ? std::launch::async : std::launch::deferred;
		std::future<std::string> onLoad;
		std::future<std::string> onSave;

		auto loadMachineState = [&](std::string&& str)
		{
			if (str.empty() == false)
			{
				// perform checks to make sure that this machine load state is compatible with this machine

				auto memUuid = memoryController->Uuid();

				if (memUuid == std::array<uint8_t, 16>{})
				{
					return make_error_code(errc::incompatible_uuid);
				}
#ifdef ENABLE_NLOHMANN_JSON
				auto json = nlohmann::json::parse(str, nullptr, false);

				if(json.is_discarded() == true)
#else
				JsonDocument json;
				auto err = deserializeJson(json, str);
				if(err)
#endif // ENABLE_NLOHMANN_JSON
				{
					return make_error_code(errc::json_parse);
				}
#ifdef ENABLE_NLOHMANN_JSON
				if(!json.contains("memory"))
#else
				if(json["memory"] == nullptr)
#endif // ENABLE_NLOHMANN_JSON
				{
					return make_error_code(errc::json_parse);
				}

				auto memory = json["memory"];
#ifdef ENABLE_NLOHMANN_JSON
				// We must contain at least rom, if we hve ram we need a uuid to match against
				if (!memory.contains("rom") || (memory.contains("ram") && !memory.contains("uuid")))
#else
				if(memory["rom"] == nullptr || (memory["ram"] != nullptr && memory["uuid"] == nullptr))
#endif // ENABLE_NLOHMANN_JSON
				{
					return make_error_code(errc::json_parse);
				}

				// The memory controllers must be the same
#ifdef ENABLE_NLOHMANN_JSON
				if (memory.contains("uuid"))
				{
					auto jsonUuid = Utils::TxtToBin("base64", "none", 16, memory["uuid"].get<std::string>());
#else
				if (memory["uuid"] == nullptr)
				{
					auto jsonUuid = Utils::TxtToBin("base64", "none", 16, memory["uuid"].as<std::string>());
#endif // ENABLE_NLOHMANN_JSON
					if (jsonUuid.size() != memUuid.size() || std::equal(jsonUuid.begin(), jsonUuid.end(), memUuid.begin()) == false)
					{
						return make_error_code(errc::incompatible_uuid);
					}
				}

#ifdef ENABLE_NLOHMANN_JSON
				if (!memory["rom"].contains("block"))
#else
				if (memory["rom"]["block"] == nullptr)
#endif // ENABLE_NLOHMANN_JSON
				{
					return make_error_code(errc::json_parse);
				}

#ifdef ENABLE_NLOHMANN_JSON
				for (auto it = memory["rom"]["block"].begin(); it < memory["rom"]["block"].end(); it++)
				{
					auto block = it.value();
					
					if (!block.contains("bytes"))
					{
						return make_error_code(errc::json_config);
					}

					auto bytes = block["bytes"].get<std::string_view>();
					int offset = 0;
					int size = 0;

					if (block.contains("offset"))
					{
						offset = block["offset"].get<int>();

						if (offset < 0)
						{
							return make_error_code(errc::json_config);
						}
					}
	
					if (block.contains("size"))
					{
						size = block["size"].get<int>();

						if (size < 0)
						{
							return make_error_code(errc::json_config);
						}
					}

					if (bytes.starts_with("file://") == true)
					{
						FILE* fin = nullptr;
						bytes.remove_prefix(strlen("file://"));
						auto err = fopen_s (&fin, std::string(bytes.begin(), bytes.end()).c_str(), "rb");

						if(err != 0)
						{
							return make_error_code(errc::incompatible_rom);
						}

						// read the entire file if the size is ommitted
						if (size == 0)
						{
							fseek(fin, 0, SEEK_END);
							size = ftell(fin);
							fseek(fin, 0, SEEK_SET);
						}			

						// only support a max of 16 bit addressing
						if(offset + size > 65535)
						{
							fclose(fin);
							return make_error_code(errc::json_config);
						}

						while(ftell(fin) < size)
						{
							uint8_t byte = 0;
							auto read = fread(&byte, 1, 1, fin);
							
							if(read == 0)
							{
								break;
							}

							memoryController->Write(offset++, byte, ioController);
						}

						err = ferror(fin);
						fclose(fin);

						if(err != 0)
						{
							return make_error_code(errc::incompatible_rom);
						}
					}
					else if (bytes.starts_with("base64://") == true)
					{
						bytes.remove_prefix(strlen("base64://"));

						if (bytes.starts_with("md5://") == true)
						{
							bytes.remove_prefix(strlen("md5://"));

							auto jsonMd5 = Utils::TxtToBin("base64",
								"none",
								size,
								std::string(bytes.begin(), bytes.end()));

							std::vector<uint8_t> rom;

							for (int addr = offset; addr < offset + size; addr++)
							{
								rom.push_back(memoryController->Read(addr, ioController));
							}

							auto romMd5 = Utils::Md5(rom.data(), rom.size());

							if (jsonMd5.size() != romMd5.size() || std::equal(jsonMd5.begin(), jsonMd5.end(), romMd5.begin()) == false)
							{
								return make_error_code(errc::incompatible_rom);
							}
						}
						else if (bytes.starts_with("zlib://") == true)
						{
							if (size <= 0 || offset + size > 65535)
							{
								return make_error_code(errc::json_config);
							}

							bytes.remove_prefix(strlen("zlib://"));

							// decompress bytes and write to memory
							auto unzip = Utils::TxtToBin("base64",
								"zlib",
								size,
								std::string(bytes.begin(), bytes.end()));

							// assert(size == unzip.length());

							for (const auto& byte : unzip)
							{
								memoryController->Write(offset++, byte, ioController);
							}							
						}
						else
						{
							if (size == 0)
							{
								size = bytes.length();
							}

							// Check to see if we will overrun the source or detination memmory
							if (offset + size > 65535 || size > bytes.length())
							{
								return make_error_code(errc::json_config);
							}

							for (int i = 0; i < size; i++)
							{
								memoryController->Write(offset++, bytes[i], ioController);
							}
						}
					}
					else
					{
						return make_error_code(errc::json_config);
					}
				}

				if (memory.contains("ram"))
				{
					if (!memory["ram"].contains("block"))
					{
						return make_error_code(errc::json_parse);
					}

					for (auto it = memory["ram"]["block"].begin(); it < memory["ram"]["block"].end(); it++)
					{
						auto block = it.value();

						if (!block.contains("bytes"))
						{
							return make_error_code(errc::json_config);
						}

						auto bytes = block["bytes"].get<std::string_view>();
						int offset = 0;
						int size = 0;

						if (block.contains("offset"))
						{
							offset = block["offset"].get<int>();

							if (offset < 0)
							{
								return make_error_code(errc::json_config);
							}
						}

						if (block.contains("size"))
						{
							size = block["size"].get<int>();

							if (size < 0)
							{
								return make_error_code(errc::json_config);
							}
						}

						if (bytes.starts_with("base64://") == true)
						{
							std::string compressor = "none";
							bytes.remove_prefix(strlen("base64://"));

							if(bytes.starts_with("zlib://") == true)
							{
								compressor = "zlib";
								bytes.remove_prefix(strlen("zlib://"));
							}

							auto ram = Utils::TxtToBin("base64",
								compressor,
								size,
								std::string(bytes.begin(), bytes.end()));

							// assert (ram.size() == size);

							// only support a max of 16 bit addressing
							if (offset + size > 65535)
							{
								return make_error_code(errc::json_config);
							}

							for (const auto& byte : ram)
							{
								memoryController->Write(offset++, byte, ioController);
							}
						}
						else
						{
							return make_error_code(errc::json_config);
						}
					}
				}

				if (json.contains("cpu"))
				{
					// if ram exists we need to check for cpu uuid compatibility
					auto err = cpu_->Load(json["cpu"].dump(), memory.contains("ram"));

					if (err)
					{
						return err;
					}
				}
			}

			return std::error_code{};
#else

#endif // ENABLE_NLOHMANN_JSON
		};

		auto checkHandler = [](std::future<std::string>& fut)
		{
			std::string str;

			if (fut.valid() == true)
			{
				auto status = fut.wait_for(nanoseconds::zero());

				if (status == std::future_status::deferred || status == std::future_status::ready)
				{
					str = fut.get();
				}
			}

			return str;
		};
#endif // ENABLE_MEEN_SAVE
		while (quit == false)
		{
			//Execute the next instruction
			auto ticks = cpu_->Execute();
			currTime = clock_->Tick(ticks);
			totalTicks += ticks;

			// Check if it is time to service interrupts
			if (totalTicks - lastTicks >= ticksPerIsr_ || ticks == 0) // when ticks is 0 the cpu is not executing (it has been halted), poll (should be less aggressive) for interrupts to unhalt the cpu
			{
				lastTicks = totalTicks;

				auto isr = ioController->ServiceInterrupts(currTime.count(), totalTicks, memoryController);

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
						ticks = cpu_->Interrupt(isr);
						currTime = clock_->Tick(ticks);
						totalTicks += ticks;
						break;
					}
					case ISR::Load:
					{
#ifdef ENABLE_MEEN_SAVE
						// If a user defined callback is set and we are not processing a load or save request
						if (onLoad_ != nullptr && onLoad.valid() == false && onSave.valid() == false)
						{
							onLoad = std::async(loadLaunchPolicy, [onLoad_]
							{
								std::string str;
								int len = 0;
								auto err = onLoad_(nullptr, &len);

								if(!err)
								{
									len++;
									str.resize(len, '\0');
									err = onLoad_(str.data(), &len);
								}

								if(err)
								{
									str.clear();
									// todo: need to have proper logging
									printf("ISR::Load failed to load the machine state: %s\n", err.message().c_str());
								}

								return str;
							});

							auto err = loadMachineState(checkHandler(onLoad));

							if(err)
							{
								printf("ISR::Load failed to load the machine state: %s\n", err.message().c_str());
							}
						}
#endif // ENABLE_MEEN_SAVE
						break;
					}
					case ISR::Save:
					{
#ifdef ENABLE_MEEN_SAVE
						// If a user defined callback is set and we are not processing a save or load request
						if (onSave_ != nullptr && onSave.valid() == false && onLoad.valid() == false)
						{
							auto err = std::error_code{};
							auto memUuid = memoryController->Uuid();

							if (opt_.Encoder() != "base64")
							{
								err = make_error_code(errc::json_config);
							}

							// This check needs to be removed for 2,0
							if (memUuid == std::array<uint8_t, 16>{})
							{
								err = make_error_code(errc::incompatible_uuid);
							}

							if(!err)
							{
								auto rm = [memoryController](std::vector<std::pair<uint16_t, uint16_t>>&& metadata)
								{
									std::vector<uint8_t> mem;

									for (const auto& m : metadata)
									{
										for (auto addr = m.first; addr < m.first + m.second; addr++)
										{
											mem.push_back(memoryController->Read(addr, nullptr));
										}
									}

									return mem;
								};

								auto ram = rm(opt_.Ram());
								auto rom = rm(opt_.Rom());
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

								onSave = std::async(saveLaunchPolicy, [onSave_, state = writeState(count)]
								{
									// TODO: this method needs to be marked as nothrow
									onSave_(state.c_str());
									// handle return error_code

									return std::string("");
								});

								checkHandler(onSave);
							}
							else
							{
								printf("ISR::Save failed: %s\n", err.message().c_str());
							}
						}
#endif // ENABLE_MEEN_SAVE
						break;
					}
					case ISR::Quit:
					{
#ifdef ENABLE_MEEN_SAVE
						// Wait for any outstanding load/save requests to complete

						if (onLoad.valid() == true)
						{
							// we are quitting, wait for the onLoad handler to complete
							auto errc = loadMachineState(onLoad.get());

							if(errc)
							{
								printf("ISR::Quit failed to load the machine state: %s\n", errc.message().c_str());
							}
						}

						if (onSave.valid() == true)
						{
							// we are quitting, wait for the onSave handler to complete
							onSave.get();
						}
#endif // ENABLE_MEEN_SAVE
						quit = true;
						break;
					}
					case ISR::NoInterrupt:
					{
#ifdef ENABLE_MEEN_SAVE
						// no interrupts pending, do any work that is outstanding
						auto errc = loadMachineState(checkHandler(onLoad));

						if(errc)
						{
							printf("ISR::NoInterrupt failed to load the machine state: %s\n", errc.message().c_str());
						}

						checkHandler(onSave);
#endif // ENABLE_MEEN_SAVE
						break;
					}
					default:
					{
						//assert(0);
						break;
					}
				}
			}
		}

		m->runTime_ = currTime.count();
	}

	std::error_code Machine::Run(uint16_t pc)
	{
		if (running_ == true)
		{
			return make_error_code(errc::busy);
		}

		if (memoryController_ == nullptr)
		{
			return make_error_code(errc::memory_controller);
		}

		if (ioController_ == nullptr)
		{
			return make_error_code(errc::io_controller);
		}

		if(clock_ == nullptr)
		{
			return make_error_code(errc::clock_resolution);
		}

		if(cpu_ == nullptr)
		{
			return make_error_code(errc::cpu);
		}

		int64_t resInTicks = 0;
		auto err = clock_->SetTickResolution(nanoseconds(opt_.ClockResolution()), &resInTicks);

		if (err)
		{
			return make_error_code(errc::clock_resolution);
		}

		// clear the memory
		for (int i = 0; i < 65535; i++)
		{
			memoryController_->Write(i, 0, ioController_.get());
		}


		cpu_->Reset(pc);
		clock_->Reset();
		runTime_ = 0;
		running_ = true;
		ticksPerIsr_ = opt_.ISRFreq() * resInTicks;

#ifdef ENABLE_MEEN_RP2040
		if(opt_.RunAsync() == true)
		{
			auto runMachineAsync = []
			{
				auto m = std::bit_cast<Machine*>(multicore_fifo_pop_blocking());
				RunMachine(m);
				multicore_fifo_push_blocking(0xFFFFFFFF);
			};

			multicore_reset_core1();
			multicore_launch_core1(runMachineAsync);
			multicore_fifo_push_blocking(std::bit_cast<uint32_t>(this));
		}
		else
		{
			RunMachine(this);
			running_ = false;
		}
#else
		auto launchPolicy = opt_.RunAsync() ? std::launch::async : std::launch::deferred;

		fut_ = std::async(launchPolicy, [this]
		{
			RunMachine(this);
		});

		if (launchPolicy == std::launch::deferred)
		{
			fut_.get();
			running_ = false;
		}
#endif // ENABLE_MEEN_RP2040
		return std::error_code{};
	}

	std::expected<uint64_t, std::error_code> Machine::WaitForCompletion()
	{
		if(running_ == true)
		{
#ifdef ENABLE_MEEN_RP2040
			auto core1Ret = multicore_fifo_pop_blocking();

			if(core1Ret != 0xFFFFFFFF)
			{
				return std::unexpected(make_error_code(errc::async));
			}
#else
			if (fut_.valid() == true)
			{
				fut_.get();
			}
			else
			{
				return std::unexpected(make_error_code(errc::async));		
			}
#endif // ENABLE_MEEN_RP2040

			running_ = false;
		}

		return runTime_;
	}

	std::error_code Machine::AttachMemoryController (IControllerPtr&& controller)
	{
		if (running_ == true)
		{
			return make_error_code(errc::busy);
		}

		if(controller == nullptr)
		{
			return make_error_code(errc::invalid_argument);
		}

		cpu_->SetMemoryController(controller.get());
		memoryController_ = std::move(controller);
		// controller = nullptr;
		return std::error_code{};
	}
		
	std::expected<IControllerPtr, std::error_code> Machine::DetachMemoryController()
	{
		if (running_ == true)
		{
			return std::unexpected<std::error_code>(make_error_code(errc::busy));
		}

		if(memoryController_ == nullptr)
		{
			return std::unexpected<std::error_code>(make_error_code(errc::memory_controller));
		}

		cpu_->SetMemoryController(nullptr);
		auto controller = std::move(memoryController_);
		memoryController_ = nullptr;
		return controller;
	}

	std::error_code Machine::AttachIoController (IControllerPtr&& controller)
	{
		if (running_ == true)
		{
			return make_error_code(errc::busy);
		}

		if(controller == nullptr)
		{
			return make_error_code(errc::invalid_argument);
		}

		cpu_->SetIoController(controller.get());
		ioController_ = std::move(controller);
		// controller = nullptr;
		return std::error_code{};
	}
		
	std::expected<IControllerPtr, std::error_code> Machine::DetachIoController()
	{
		if (running_ == true)
		{
			return std::unexpected<std::error_code>(make_error_code(errc::busy));
		}

		if(ioController_ == nullptr)
		{
			return std::unexpected<std::error_code>(make_error_code(errc::io_controller));
		}

		cpu_->SetIoController(nullptr);
		auto controller = std::move(ioController_);
		ioController_ = nullptr;
		return controller;
	}

	std::error_code Machine::OnSave(std::function<errc(const char* json)>&& onSave)
	{
#ifdef ENABLE_MEEN_SAVE
		if (running_ == true)
		{
			return make_error_code(errc::busy);
		}

		onSave_ = std::move(onSave);
		return std::error_code{};
#else
		return make_error_code(errc::not_implemented);
#endif // ENABLE_MEEN_SAVE
	}

	std::error_code Machine::OnLoad(std::function<errc(char* json, int* jsonLen)>&& onLoad)
	{
#ifdef ENABLE_MEEN_SAVE
		if (running_ == true)
		{
			return make_error_code(errc::busy);
		}

		onLoad_ = std::move(onLoad);
		return std::error_code{};
#else
		return make_error_code(errc::not_implemented);
#endif // ENABLE_MEEN_SAVE
	}

	ControllerDeleter::ControllerDeleter(bool del)
	{
		delete_ = del;
	}
	
	void ControllerDeleter::operator()(IController* controller)
	{
		if(delete_ == true)
		{
			delete controller;
		}
	};
} // namespace meen