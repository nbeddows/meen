/*
Copyright (c) 2021-2025 Nicolas Beddows <nicolas.beddows@gmail.com>

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
#include <charconv>
#include <cinttypes>
#include <stdio.h>
#ifdef ENABLE_MEEN_RP2040
#include <pico/multicore.h>
#endif // ENABLE_MEEN_RP2040
#ifdef ENABLE_NLOHMANN_JSON
#include <nlohmann/json.hpp>
#else
#define ARDUINOJSON_ENABLE_STRING_VIEW 1
#include <ArduinoJson.h>
#endif // ENABLE_NLOHMANN_JSON
#include "meen/utils/Utils.h"
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
		auto& romMetadata = m->romMetadata_;
		auto& ramMetadata = m->ramMetadata_;
		auto& onLoad_ = m->onLoad_;
		auto& opt_ = m->opt_;
#ifndef ENABLE_MEEN_RP2040
		auto loadLaunchPolicy = opt_.LoadAsync() ? std::launch::async : std::launch::deferred;
		std::future<std::string> onLoad;
#endif // ENABLE_MEEN_RP2040
		auto memoryController = m->memoryController_.get();
		auto loadMachineState = [&](std::string&& str)
		{
			if (str.empty() == false)
			{
#ifdef ENABLE_NLOHMANN_JSON
				auto json = nlohmann::json::parse(str, nullptr, false);

				if(json.is_discarded() == true)
#else
				JsonDocument json;
				auto je = deserializeJson(json, str);
				if(je)
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
				// We must contain at least rom, if we have ram we need a uuid to match against
				if (!memory.contains("rom")
#ifdef ENABLE_MEEN_SAVE
				|| (memory.contains("ram") && !memory.contains("uuid"))
#endif // ENABLE_MEEN_SAVE
				)
#else
				if(memory["rom"] == nullptr
#ifdef ENABLE_MEEN_SAVE
				|| (memory["ram"] != nullptr && memory["uuid"] == nullptr)
#endif // ENABLE_MEEN_SAVE
				)
#endif // ENABLE_NLOHMANN_JSON
				{
					return make_error_code(errc::json_parse);
				}

#ifdef ENABLE_MEEN_SAVE
				// The memory controllers must be the same
#ifdef ENABLE_NLOHMANN_JSON
				if (memory.contains("uuid"))
				{
					auto sv = memory["uuid"].get<std::string_view>();
#else
				if (memory["uuid"])
				{
					auto sv = memory["uuid"].as<std::string_view>();
#endif // ENABLE_NLOHMANN_JSON
					auto memUuid = memoryController->Uuid();

					if (sv.starts_with("base64://"))
					{
						sv.remove_prefix(strlen("base64://"));
					}
					else
					{
						return make_error_code(errc::json_config);
					}

					auto jsonUuid = Utils::TxtToBin("base64", "none", 16, std::string(sv));

					if (jsonUuid.size() != memUuid.size() || std::equal(jsonUuid.begin(), jsonUuid.end(), memUuid.begin()) == false)
					{
						return make_error_code(errc::incompatible_uuid);
					}
				}
#endif // ENABLE_MEEN_SAVE

				bool clear = true;

#ifdef ENABLE_NLOHMANN_JSON
				auto loadRom = [&clear, memoryController, ioController, &romMetadata](const nlohmann::json& block)
				{
					if (!block.contains("bytes"))
#else
				auto loadRom = [&clear, memoryController, ioController, &romMetadata](const JsonVariantConst& block)
				{
					if(!block["bytes"])
#endif // ENABLE_NLOHMANN_JSON
					{
						return make_error_code(errc::json_config);
					}

#ifdef ENABLE_NLOHMANN_JSON
					auto bytes = block["bytes"].get<std::string_view>();
#else
					auto bytes = block["bytes"].as<std::string_view>();
#endif //ENABLE_NLOHMANN_JSON
					int offset = 0;
					int size = 0;

#ifdef ENABLE_NLOHMANN_JSON
					if (block.contains("offset"))
					{
						offset = block["offset"].get<int>();
#else
					if (block["offset"])
					{
						offset = block["offset"].as<int>();

#endif // ENABLE_NLOHMANN_JSON
						if (offset < 0)
						{
							return make_error_code(errc::json_config);
						}
					}

#ifdef ENABLE_NLOHMANN_JSON
					if (block.contains("size"))
					{
						size = block["size"].get<int>();
#else
					if (block["size"])
					{
						size = block["size"].as<int>();
#endif // ENABLE_NLOHMANN_JSON
						if (size < 0)
						{
							return make_error_code(errc::json_config);
						}
					}
#ifndef ENABLE_MEEN_RP2040
					if (bytes.starts_with("file://") == true)
					{
						bytes.remove_prefix(strlen("file://"));
						FILE* fin = fopen(std::string(bytes).c_str(), "rb");

						if(fin == nullptr)
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
						if(offset + size > 0xFFFF)
						{
							fclose(fin);
							return make_error_code(errc::json_config);
						}

						for (int i = offset; ftell(fin) < size; i++)
						{
							uint8_t byte = 0;
							auto read = fread(&byte, 1, 1, fin);

							if(read == 0)
							{
								break;
							}

							memoryController->Write(i, byte, ioController);
						}

						auto err = ferror(fin);
						fclose(fin);

						if(err != 0)
						{
							return make_error_code(errc::incompatible_rom);
						}

						if (clear == true)
						{
							romMetadata.clear();
							clear = false;
						}

						romMetadata.emplace(offset, size);
					}
					else
#endif // ENABLE_MEEN_RP2040
#ifdef ENABLE_BASE64
					if (bytes.starts_with("base64://") == true)
					{
						bytes.remove_prefix(strlen("base64://"));
#ifdef ENABLE_MEEN_SAVE
						if (bytes.starts_with("md5://") == true)
						{
							bytes.remove_prefix(strlen("md5://"));

							if (size == 0)
							{
								size = bytes.length();
							}

							auto jsonMd5 = Utils::TxtToBin("base64",
								"none",
								size,
								std::string(bytes.begin(), bytes.end()));

							std::vector<uint8_t> rom;

							for (const auto& rm : romMetadata)
							{
								for (int addr = rm.first; addr < rm.first + rm.second; addr++)
								{
									rom.push_back(memoryController->Read(addr, ioController));
								}
							}

							auto romMd5 = Utils::Md5(rom.data(), rom.size());

							if (jsonMd5.size() != romMd5.size() || std::equal(jsonMd5.begin(), jsonMd5.end(), romMd5.begin()) == false)
							{
								return make_error_code(errc::incompatible_rom);
							}
						}
						else
#endif // ENABLE_MEEN_SAVE
						{
							const char* compressor;

							if (bytes.starts_with("zlib://") == true)
							{
#ifdef ENABLE_ZLIB
								compressor = "zlib";

								if (size <= 0)
								{
									return make_error_code(errc::json_config);
								}

								bytes.remove_prefix(strlen("zlib://"));
#else
								return make_error_code(errc::no_zlib);
#endif // ENABLE_ZLIB
							}
							else
							{
								compressor = "none";

								if (size <= 0)
								{
									size = bytes.length();
								}
							}

							// decompress bytes and write to memory
							auto romBytes = Utils::TxtToBin("base64",
								compressor,
								size,
								std::string(bytes.begin(), bytes.end()));

							// only support a max of 16 bit addressing
							if (offset + romBytes.size() > 0xFFFF)
							{
								return make_error_code(errc::json_config);
							}

							for (int i = 0; i < romBytes.size(); i++)
							{
								memoryController->Write(offset + i, romBytes[i], ioController);
							}

							if (clear == true)
							{
								romMetadata.clear();
								clear = false;
							}

							romMetadata.emplace(offset, static_cast<uint16_t>(romBytes.size()));
						}
					}
					else
#endif // ENABLE_BASE64
					if (bytes.starts_with("mem://") == true)
					{
						bytes.remove_prefix(strlen("mem://"));

						if (size <= 0)
						{
							return make_error_code(errc::json_config);
						}

						int64_t value = 0;
						auto [ptr, ec] = std::from_chars (bytes.data(), bytes.data() + bytes.size(), value, 16);

						if (ec != std::errc() || ptr != bytes.data() + bytes.size())
						{
							return make_error_code(errc::json_config);
						}

						auto romBytes = reinterpret_cast<uint8_t*>(value);

						// only support a max of 16 bit addressing
						if (offset + size > 0xFFFF)
						{
							return make_error_code(errc::json_config);
						}

						for (int i = 0; i < size; i++)
						{
							memoryController->Write(offset + i, romBytes[i], ioController);
						}

						if (clear == true)
						{
							romMetadata.clear();
							clear = false;
						}

						romMetadata.emplace(offset, static_cast<uint16_t>(size));
					}
					else
					{
						return make_error_code(errc::json_config);
					}

					return std::error_code{};
				};

#ifdef ENABLE_NLOHMANN_JSON
				if (memory["rom"].contains("block"))
				{
					for (const auto& block : memory["rom"]["block"])
#else
				if (memory["rom"]["block"])
				{
					for (const auto& block : memory["rom"]["block"].as<JsonArrayConst>())
#endif // ENABLE_NLOHMANN_JSON
					{
						auto err = loadRom(block);

						if (err)
						{
							return err;
						}
					}
				}
				else
				{
					auto err = loadRom(memory["rom"]);

					if (err)
					{
						return err;
					}
				}

				int offset = 0;
				ramMetadata.clear();

				// store an ascending sorted ram map with offset as the key (derived from the rom)
				for (const auto& r : romMetadata)
				{
					if (offset < r.first)
					{
						ramMetadata.emplace(offset, r.first - offset);
					}

					offset = r.first + r.second;
				}

				// add the last ram block
				if (offset < 0xFFFF)
				{
					ramMetadata.emplace(offset, 0xFFFF - offset);
				}

#ifdef ENABLE_MEEN_SAVE
#ifdef ENABLE_NLOHMANN_JSON
				if (memory.contains("ram"))
#else
				if (memory["ram"])
#endif // ENABLE_NLOHMANN_JSON
				{
					// flat ram, decompress the entire ram
#ifdef ENABLE_NLOHMANN_JSON
					if (!memory["ram"].contains("bytes"))
#else
					if (!memory["ram"]["bytes"])
#endif // ENABLE_NLOHMANN_JSON
					{
						return make_error_code(errc::json_config);
					}

#ifdef ENABLE_NLOHMANN_JSON
					auto bytes = memory["ram"]["bytes"].get<std::string_view>();
#else
					auto bytes = memory["ram"]["bytes"].as<std::string_view>();
#endif // ENABLE_NLOHMANN_JSON
					std::vector<uint8_t> ram;
					auto size = std::accumulate (ramMetadata.begin(), ramMetadata.end(), 0, [](int accumulator, const std::pair<uint16_t, uint16_t>& metadata)
					{
						return accumulator + metadata.second;
					});
#ifdef ENABLE_BASE64
					if (bytes.starts_with("base64://") == true)
					{
						std::string compressor = "none";
						bytes.remove_prefix(strlen("base64://"));

						if (bytes.starts_with("zlib://") == true)
						{
#ifdef ENABLE_ZLIB
							compressor = "zlib";
							bytes.remove_prefix(strlen("zlib://"));
#else
							return make_error_code(errc::no_zlib);
#endif // ENABLE_ZLIB
						}

						ram = Utils::TxtToBin("base64",
							compressor,
							size,
							std::string(bytes));

						// if ram is empty, return no_zlib ... TxtToBin, BinToTxt need to return std::expected
						if (ram.empty() == true || ram.size() != size)
						{
							return make_error_code(errc::incompatible_ram);
						}
					}
					else
#endif // ENABLE_BASE64
					{
						return make_error_code(errc::json_config);
					}

					auto ramIt = ram.begin();

					// loop ramMetadata writing the size bytes from decompressed ram to memory at offset
					for (const auto& rm : ramMetadata)
					{
						// only support a max of 16 bit addressing
						if (rm.first + rm.second > 0xFFFF)
						{
							return make_error_code(errc::json_config);
						}

						for (int addr = rm.first; addr < rm.first + rm.second; addr++)
						{
							memoryController->Write(addr, *ramIt++, ioController);
						}
					}
				}
				else
#endif // ENABLE_MEEN_SAVE
				{
					// make sure the ram is clear
					for (const auto& rm : ramMetadata)
					{
						for (int addr = rm.first; addr < rm.first + rm.second; addr++)
						{
							memoryController->Write(addr, 0x00, ioController);
						}
					}
				}

#ifdef ENABLE_NLOHMANN_JSON
				if (json.contains("cpu"))
				{
					// if ram exists we need to check for cpu uuid compatibility
					auto err = cpu_->Load(json["cpu"].dump(),
#ifdef ENABLE_MEEN_SAVE
					memory.contains("ram")
#else
					false
#endif // ENABLE_MEEN_SAVE
					);
#else
				if (json["cpu"])
				{
					std::string cpuStr;
					serializeJson(json["cpu"], cpuStr);

					if(cpuStr.empty() == true)
					{
						return make_error_code(errc::json_parse);
					}

					auto err = cpu_->Load(std::move(cpuStr),
#ifdef ENABLE_MEEN_SAVE
					memory["ram"]
#else
					false
#endif // ENABLE_MEEN_SAVE
					);
#endif // ENABLE_NLOHMANN_JSON
					if (err)
					{
						return err;
					}
				}
			}

			return std::error_code{};
		};

#ifndef ENABLE_MEEN_RP2040
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
#endif // ENABLE_MEEN_RP2040
#ifdef ENABLE_MEEN_SAVE
		auto& onSave_ = m->onSave_;
		auto saveLaunchPolicy = opt_.SaveAsync() ? std::launch::async : std::launch::deferred;
		std::future<std::string> onSave;
#endif // ENABLE_MEEN_SAVE
		int ticks = 0;

		while (quit == false)
		{
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
						// If a user defined callback is set and we are not processing a load or save request
						if (onLoad_ != nullptr
#ifndef ENABLE_MEEN_RP2040
						&& onLoad.valid() == false
#endif // ENABLE_MEEN_RP2040
#ifdef ENABLE_MEEN_SAVE
						&& onSave.valid() == false
#endif // ENABLE_MEEN_SAVE
						)
						{
#ifndef ENABLE_MEEN_RP2040
							onLoad = std::async(loadLaunchPolicy, [onLoad_]
							{
#endif // ENABLE_MEEN_RP2040
								std::string str;
								int len = 0;
								auto e = onLoad_(nullptr, &len);

								if(!e)
								{
									len++;
									str.resize(len, '\0');
									e = onLoad_(str.data(), &len);
								}

								if(e)
								{
									str.clear();
									// todo: need to have proper logging
									printf("ISR::Load failed to load the machine state: %s\n", e.message().c_str());
								}
#ifndef ENABLE_MEEN_RP2040
								return str;
							});

							auto err = loadMachineState(checkHandler(onLoad));
#else
							auto err = loadMachineState(std::move(str));
#endif // ENABLE_MEEN_RP2040

							if(err)
							{
								printf("ISR::Load failed to load the machine state: %s\n", err.message().c_str());
							}
						}
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

							if(!err)
							{
								auto rm = [memoryController](const std::map<uint16_t, uint16_t>& metadata)
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

								auto ram = rm(ramMetadata);
								auto rom = rm(romMetadata);
								auto fmtStr = R"({"cpu":%s,"memory":{"uuid":"%s://%s","rom":{"bytes":"%s://md5://%s"},"ram":{"size":%d,"bytes":"%s://%s://%s"}}})";
								auto romMd5 = Utils::Md5(rom.data(), rom.size());
								size_t dataSize = 0;

								// todo - replace snprintf with std::format
								auto writeState = [&]()
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
										opt_.Encoder().c_str(),
										Utils::BinToTxt("base64", "none", memUuid.data(), memUuid.size()).c_str(),
										opt_.Encoder().c_str(),
										Utils::BinToTxt("base64", "none", romMd5.data(), romMd5.size()).c_str(),
										ram.size(), opt_.Encoder().c_str(), opt_.Compressor().c_str(),
										Utils::BinToTxt(opt_.Encoder(), opt_.Compressor(), ram.data(), ram.size()).c_str()) + 1;
									return str;
								};

								writeState();

								onSave = std::async(saveLaunchPolicy, [onSave_, state = writeState()]
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
						// Wait for any outstanding load/save requests to complete
#ifndef ENABLE_MEEN_RP2040
						if (onLoad.valid() == true)
						{
							// we are quitting, wait for the onLoad handler to complete
							auto err = loadMachineState(onLoad.get());

							if(err)
							{
								printf("ISR::Quit failed to load the machine state: %s\n", err.message().c_str());
							}
						}
#endif // ENABLE_MEEN_RP2040
#ifdef ENABLE_MEEN_SAVE
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
#ifndef ENABLE_MEEN_RP2040
						// no interrupts pending, do any work that is outstanding
						auto errc = loadMachineState(checkHandler(onLoad));

						if(errc)
						{
							printf("ISR::NoInterrupt failed to load the machine state: %s\n", errc.message().c_str());
						}
#endif // ENABLE_MEEN_RP2040
#ifdef ENABLE_MEEN_SAVE
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

			//Execute the next instruction
			ticks = cpu_->Execute();
			currTime = clock_->Tick(ticks);
			totalTicks += ticks;
		}

		m->runTime_ = currTime.count();
	}

	std::error_code Machine::Run()
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

		cpu_->Reset();
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
		if (running_ == true)
		{
			return make_error_code(errc::busy);
		}

		onLoad_ = std::move(onLoad);
		return std::error_code{};
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