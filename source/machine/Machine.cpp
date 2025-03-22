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
#include <numeric>
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
			return HandleError(errc::busy, std::source_location::current());
		}
	
		auto err = opt_.SetOptions(options);

		if (err)
		{
			HandleError(err, std::source_location::current());		
		}

		return err;
	}

	std::error_code Machine::HandleError(std::error_code err, std::source_location&& sl)
	{
		if (onError_)
		{
			onError_(err, sl.file_name(), sl.function_name(), sl.line(), sl.column(), ioController_.get());
		}

		return err;
	}

	std::error_code Machine::HandleError(errc ec, std::source_location&& sl)
	{
		return HandleError(make_error_code(ec), std::move(sl));
	}

	void RunMachine(meen::Machine* m)
	{
		auto currTime = nanoseconds::zero();
		int64_t totalTicks = 0;
		int64_t lastTicks = 0;
		auto ticksPerIsr = 0ull;
#ifndef ENABLE_MEEN_RP2040
		auto loadLaunchPolicy = m->opt_.LoadAsync() ? std::launch::async : std::launch::deferred;
		std::future<std::string> onLoad;
#endif // ENABLE_MEEN_RP2040
		auto loadMachineState = [&](std::string&& str)
		{
			if (str.empty() == false)
			{
#ifdef ENABLE_NLOHMANN_JSON
				nlohmann::json json;
#else
				JsonDocument json;
#endif // ENABLE_NLOHMANN_JSON
				auto parseJsonStr = [&json, m](std::string&& str)
				{
					auto err = std::error_code{};
#ifdef ENABLE_NLOHMANN_JSON
					json = nlohmann::json::parse(str, nullptr, false);

					if(json.is_discarded() == true)
#else
					auto je = deserializeJson(json, str);

					if(je)
#endif // ENABLE_NLOHMANN_JSON
					{
						err = m->HandleError(errc::json_parse, std::source_location::current());
					}

					return err;
				};

				if (str.starts_with("file://") == true)
				{
					str.erase(0, strlen("file://"));
					auto fin = fopen(str.c_str(), "r");

					if (fin != nullptr)
					{
#ifdef ENABLE_NLOHMANN_JSON
						json = nlohmann::json::parse(fin, nullptr, false);
						fclose(fin);

						if(json.is_discarded() == true)
#else
						fseek(fin, 0, SEEK_END);
						std::string jsonStr(ftell(fin), '\0');
						fseek(fin, 0, SEEK_SET);
						fread(jsonStr.data(), 1, jsonStr.size(), fin);
						fclose(fin);

						auto je = deserializeJson(json, jsonStr);

						if (je)
#endif // ENABLE_NLOHMANN_JSON
						{
							return m->HandleError(errc::json_parse, std::source_location::current());
						}
					}
					else
					{
						return m->HandleError(errc::incompatible_rom, std::source_location::current());
					}
				}
				else if (str.starts_with("json://") == true)
				{
					str.erase(0, strlen("json://"));

					auto err = parseJsonStr(std::move(str));

					if (err)
					{
						return err;
					}
				}
				else
				{
					auto err = parseJsonStr(std::move(str));

					if (err)
					{
						return err;
					}
				}
#ifdef ENABLE_NLOHMANN_JSON
				if(!json.contains("memory"))
#else
				if(json["memory"] == nullptr)
#endif // ENABLE_NLOHMANN_JSON
				{
					return m->HandleError(errc::json_parse, std::source_location::current());
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
					return m->HandleError(errc::json_parse, std::source_location::current());
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
					auto memUuid = m->memoryController_->Uuid();

					if (sv.starts_with("base64://") == false)
					{
						return m->HandleError(errc::uri_scheme, std::source_location::current());
					}

					sv.remove_prefix(strlen("base64://"));
					auto jsonUuid = Utils::TxtToBin("base64", "none", 16, std::string(sv));

					if (!jsonUuid)
					{
						return m->HandleError(jsonUuid.error(), std::source_location::current());
					}

					if (jsonUuid.value().size() != memUuid.size() || std::equal(jsonUuid.value().begin(), jsonUuid.value().end(), memUuid.begin()) == false)
					{
						return m->HandleError(errc::incompatible_uuid, std::source_location::current());
					}
				}
#endif // ENABLE_MEEN_SAVE

				bool clear = true;

#ifdef ENABLE_NLOHMANN_JSON
				auto loadRom = [&clear, m/*memoryController, ioController, &romMetadata*/](const nlohmann::json& block, std::string_view&& scheme, std::string_view&& directory)
				{
					if (!block.contains("bytes"))
#else
				auto loadRom = [&clear, m/*memoryController, ioController, &romMetadata*/](const JsonVariantConst& block, std::string_view&& scheme, std::string_view&& directory)
				{
					if(!block["bytes"])
#endif // ENABLE_NLOHMANN_JSON
					{
						return m->HandleError(errc::json_config, std::source_location::current());
					}

#ifdef ENABLE_NLOHMANN_JSON
					auto bytes = block["bytes"].get<std::string_view>();
#else
					auto bytes = block["bytes"].as<std::string_view>();
#endif //ENABLE_NLOHMANN_JSON
					int offset = 0;
					int size = 0;
					auto err = std::error_code{};

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
							return m->HandleError(errc::json_config, std::source_location::current());
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
							return m->HandleError(errc::json_config, std::source_location::current());
						}
					}

					auto loadFromFile = [m,/*ioController, memoryController, &romMetadata,*/ &clear, offset, &size](std::string_view&& resource)
					{
						FILE* fin = fopen(std::string(resource).c_str(), "rb");

						if(fin == nullptr)
						{
							return m->HandleError(errc::incompatible_rom, std::source_location::current());
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
							return m->HandleError(errc::json_config, std::source_location::current());
						}

						for (int i = offset; ftell(fin) < size; i++)
						{
							uint8_t byte = 0;
							auto read = fread(&byte, 1, 1, fin);

							if(read == 0)
							{
								break;
							}

							m->memoryController_->Write(i, byte, m->ioController_.get());
						}

						auto err = ferror(fin);
						fclose(fin);

						if(err != 0)
						{
							return m->HandleError(errc::incompatible_rom, std::source_location::current());
						}

						if (clear == true)
						{
							m->romMetadata_.clear();
							clear = false;
						}

						m->romMetadata_.emplace(offset, size);

						return std::error_code{};
					};

					auto loadFromBase64 = [m,/*ioController, memoryController, &romMetadata,*/ &clear, offset](std::string_view&& resource, const char* compressor, int size)
					{
						// decompress bytes and write to memory
						auto romBytesEx = Utils::TxtToBin("base64",
							compressor,
							size,
							std::string(resource.begin(), resource.end()));

						if (!romBytesEx)
						{
							return m->HandleError(romBytesEx.error(), std::source_location::current());
						}

						auto romBytes = std::move(romBytesEx.value());

						// only support a max of 16 bit addressing
						if (offset + romBytes.size() > 0xFFFF)
						{
							return m->HandleError(errc::json_config, std::source_location::current());
						}

						for (int i = 0; i < romBytes.size(); i++)
						{
							m->memoryController_->Write(offset + i, romBytes[i], m->ioController_.get());
						}

						if (clear == true)
						{
							m->romMetadata_.clear();
							clear = false;
						}

						m->romMetadata_.emplace(offset, static_cast<uint16_t>(romBytes.size()));

						return std::error_code{};
					};

					auto loadFromMem = [m,/*ioController, memoryController, &romMetadata, */&clear, offset, size](std::string_view&& resource)
					{
						if (size <= 0)
						{
							return m->HandleError(errc::json_config, std::source_location::current());
						}

						int64_t value = 0;
						auto [ptr, ec] = std::from_chars (resource.data(), resource.data() + resource.size(), value, 16);

						if (ec != std::errc() || ptr != resource.data() + resource.size())
						{
							return m->HandleError(errc::json_config, std::source_location::current());
						}

						auto romBytes = reinterpret_cast<uint8_t*>(value);

						// only support a max of 16 bit addressing
						if (offset + size > 0xFFFF)
						{
							return m->HandleError(errc::json_config, std::source_location::current());
						}

						for (int i = 0; i < size; i++)
						{
							m->memoryController_->Write(offset + i, romBytes[i], m->ioController_.get());
						}

						if (clear == true)
						{
							m->romMetadata_.clear();
							clear = false;
						}

						m->romMetadata_.emplace(offset, static_cast<uint16_t>(size));

						return std::error_code{};
					};

					if (bytes.starts_with("file://") == true)
					{
						bytes.remove_prefix(strlen("file://"));

						err = loadFromFile(std::move(bytes));
					}
					else if (bytes.starts_with("base64://") == true)
					{
						bytes.remove_prefix(strlen("base64://"));

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

							if (!jsonMd5)
							{
								return m->HandleError(jsonMd5.error(), std::source_location::current());
							}

							std::vector<uint8_t> rom;

							for (const auto& rm : m->romMetadata_)
							{
								for (int addr = rm.first; addr < rm.first + rm.second; addr++)
								{
									rom.push_back(m->memoryController_->Read(addr, m->ioController_.get()));
								}
							}

							auto romMd5 = Utils::Md5(rom.data(), rom.size());

							if (jsonMd5.value().size() != romMd5.size() || std::equal(jsonMd5.value().begin(), jsonMd5.value().end(), romMd5.begin()) == false)
							{
								return m->HandleError(errc::incompatible_rom, std::source_location::current());
							}
						}
						else
						{
							const char* compressor;

							if (bytes.starts_with("zlib://") == true)
							{
								compressor = "zlib";

								if (size <= 0)
								{
									return m->HandleError(errc::json_config, std::source_location::current());
								}

								bytes.remove_prefix(strlen("zlib://"));
							}
							else
							{
								compressor = "none";

								if (size <= 0)
								{
									size = bytes.length();
								}
							}

							err = loadFromBase64(std::move(bytes), compressor, size);
						}
					}
					else if (bytes.starts_with("mem://") == true)
					{
						bytes.remove_prefix(strlen("mem://"));

						err = loadFromMem(std::move(bytes));
					}
					else
					{
						if (scheme == "file://")
						{
							if(directory.empty() == false)
							{
								err = loadFromFile(std::string(directory) + "/" + std::string(bytes));
							}
							else
							{
								err = loadFromFile(std::move(bytes));
							}
						}
						else if (scheme == "base64://")
						{
							if (size <= 0)
							{
								size = bytes.length();
							}

							err = loadFromBase64(std::move(bytes), "none", size);
						}
						else if (scheme == "mem://")
						{
							err = loadFromMem(std::move(bytes));
						}
						else
						{
							return m->HandleError(errc::uri_scheme, std::source_location::current());
						}
					}

					return err;
				};

#ifdef ENABLE_NLOHMANN_JSON
				if (memory["rom"].contains("block"))
				{
					for (const auto& block : memory["rom"]["block"])
					{
						std::string_view scheme = "";
						std::string_view directory = "";

						if (memory["rom"].contains("scheme"))
						{
							scheme = memory["rom"]["scheme"].get<std::string_view>();
						}
				
						if (memory["rom"].contains("directory"))
						{
							directory = memory["rom"]["directory"].get<std::string_view>();
						}
#else
				if (memory["rom"]["block"])
				{
					for (const auto& block : memory["rom"]["block"].as<JsonArrayConst>())
					{
						std::string_view scheme = "";
						std::string_view directory = "";

						if (memory["rom"]["scheme"])
						{
							scheme = memory["rom"]["scheme"].as<std::string_view>();
						}

						if (memory["rom"]["directory"])
						{
							directory = memory["rom"]["directory"].as<std::string_view>();
						}
#endif // ENABLE_NLOHMANN_JSON
						auto err = loadRom(block, std::move(scheme), std::move(directory));

						if (err)
						{
							return err;
						}
					}
				}
				else
				{
					auto err = loadRom(memory["rom"], "", "");

					if (err)
					{
						return err;
					}
				}

				int offset = 0;
				m->ramMetadata_.clear();

				// store an ascending sorted ram map with offset as the key (derived from the rom)
				for (const auto& r : m->romMetadata_)
				{
					if (offset < r.first)
					{
						m->ramMetadata_.emplace(offset, r.first - offset);
					}

					offset = r.first + r.second;
				}

				// add the last ram block
				if (offset < 0xFFFF)
				{
					m->ramMetadata_.emplace(offset, 0xFFFF - offset);
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
						return m->HandleError(errc::json_config, std::source_location::current());
					}

#ifdef ENABLE_NLOHMANN_JSON
					auto bytes = memory["ram"]["bytes"].get<std::string_view>();
#else
					auto bytes = memory["ram"]["bytes"].as<std::string_view>();
#endif // ENABLE_NLOHMANN_JSON
					std::vector<uint8_t> ram;
					auto size = std::accumulate (m->ramMetadata_.begin(), m->ramMetadata_.end(), 0, [](int accumulator, const std::pair<uint16_t, uint16_t>& metadata)
					{
						return accumulator + metadata.second;
					});

					if (bytes.starts_with("base64://") == true)
					{
						std::string compressor = "none";
						bytes.remove_prefix(strlen("base64://"));

						if (bytes.starts_with("zlib://") == true)
						{
							compressor = "zlib";
							bytes.remove_prefix(strlen("zlib://"));
						}

						auto ramEx = Utils::TxtToBin("base64",
							compressor,
							size,
							std::string(bytes));

						if (!ramEx)
						{
							return m->HandleError(ramEx.error(), std::source_location::current());
						}

						ram = std::move(ramEx.value());

						if (ram.size() != size)
						{
							return m->HandleError(errc::incompatible_ram, std::source_location::current());							
						}
					}
					else
					{
						return m->HandleError(errc::uri_scheme, std::source_location::current());
					}

					auto ramIt = ram.begin();

					// loop ramMetadata writing the size bytes from decompressed ram to memory at offset
					for (const auto& rm : m->ramMetadata_)
					{
						// only support a max of 16 bit addressing
						if (rm.first + rm.second > 0xFFFF)
						{
							return m->HandleError(errc::json_config, std::source_location::current());
						}

						for (int addr = rm.first; addr < rm.first + rm.second; addr++)
						{
							m->memoryController_->Write(addr, *ramIt++, m->ioController_.get());
						}
					}
				}
				else
#endif // ENABLE_MEEN_SAVE
				{
					// make sure the ram is clear
					for (const auto& rm : m->ramMetadata_)
					{
						for (int addr = rm.first; addr < rm.first + rm.second; addr++)
						{
							m->memoryController_->Write(addr, 0x00, m->ioController_.get());
						}
					}
				}

#ifdef ENABLE_NLOHMANN_JSON
				if (json.contains("cpu"))
				{
					// if ram exists we need to check for cpu uuid compatibility
					auto err = m->cpu_->Load(json["cpu"].dump(),
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
						return m->HandleError(errc::json_parse, std::source_location::current());
					}

					auto err = m->cpu_->Load(std::move(cpuStr),
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
		auto saveLaunchPolicy = m->opt_.SaveAsync() ? std::launch::async : std::launch::deferred;
		std::future<std::string> onSave;
#endif // ENABLE_MEEN_SAVE
		int ticks = 0;
		auto serviceInterrupts = [&]
		{
			bool quit = false;
			auto isr = m->ioController_->ServiceInterrupts(currTime.count(), totalTicks, m->memoryController_.get());

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
					ticks = m->cpu_->Interrupt(isr);
					currTime = m->clock_->Tick(ticks);
					totalTicks += ticks;
					break;
				}
				case ISR::Load:
				{
					// If a user defined callback is set and we are not processing a load or save request
					if (m->onLoad_ != nullptr
#ifndef ENABLE_MEEN_RP2040
						&& onLoad.valid() == false
#endif // ENABLE_MEEN_RP2040
#ifdef ENABLE_MEEN_SAVE
						&& onSave.valid() == false
#endif // ENABLE_MEEN_SAVE
					)
					{
#ifndef ENABLE_MEEN_RP2040
						onLoad = std::async(loadLaunchPolicy, [m]
						{
#endif // ENABLE_MEEN_RP2040
							int len = m->opt_.MaxLoadStateLength();
							std::string str(len, '\0');

							auto e = m->onLoad_(str.data(), &len, m->ioController_.get());

							if(e)
							{
								str.clear();
								m->HandleError(e, std::source_location::current());
							}

							str.resize(len);
#ifndef ENABLE_MEEN_RP2040
							return str;
						});

						loadMachineState(checkHandler(onLoad));
#else
						loadMachineState(std::move(str));
#endif // ENABLE_MEEN_RP2040
					}
					break;
				}
				case ISR::Save:
				{
#ifdef ENABLE_MEEN_SAVE
					// If a user defined callback is set and we are not processing a save or load request
					if (m->onSave_ != nullptr && onSave.valid() == false && onLoad.valid() == false)
					{
						auto memUuid = m->memoryController_->Uuid();
						auto memUuidTxt = Utils::BinToTxt(m->opt_.Encoder(), "none", memUuid.data(), memUuid.size());

						if (memUuidTxt)
						{
							auto rm = [memoryController = m->memoryController_.get()](const std::map<uint16_t, uint16_t>& metadata)
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

							auto rom = rm(m->romMetadata_);
							auto romMd5 = Utils::Md5(rom.data(), rom.size());
							auto romMd5Txt = Utils::BinToTxt(m->opt_.Encoder(), "none", romMd5.data(), romMd5.size());

							if (romMd5Txt)
							{
								auto ram = rm(m->ramMetadata_);
								auto ramTxt = Utils::BinToTxt(m->opt_.Encoder(), m->opt_.Compressor(), ram.data(), ram.size());

								if (ramTxt)
								{
									auto cpuStateTxt = m->cpu_->Save();

									if (cpuStateTxt)
									{
										size_t dataSize = 0;
										auto fmtStr = R"({"cpu":%s,"memory":{"uuid":"%s://%s","rom":{"bytes":"%s://md5://%s"},"ram":{"size":%d,"bytes":"%s://%s://%s"}}})";

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
												cpuStateTxt.value().c_str(),
												m->opt_.Encoder().c_str(),
												memUuidTxt.value().c_str(),
												m->opt_.Encoder().c_str(),
												romMd5Txt.value().c_str(),
												ram.size(), m->opt_.Encoder().c_str(), m->opt_.Compressor().c_str(),
												ramTxt.value().c_str()) + 1;

											return str;
										};

										writeState();

										onSave = std::async(saveLaunchPolicy, [m, state = writeState()]
										{
											// TODO: this method needs to be marked as nothrow
											auto e = m->onSave_(state.c_str(), m->ioController_.get());
											
											if (e)
											{
												m->HandleError(e, std::source_location::current());
											}

											return std::string("");
										});

										checkHandler(onSave);
									}
									else
									{
										m->HandleError(cpuStateTxt.error(), std::source_location::current());
									}
								}
								else
								{
									m->HandleError(ramTxt.error(), std::source_location::current());
								}
							}
							else
							{
								m->HandleError(romMd5Txt.error(), std::source_location::current());
							}
						}
						else
						{
							m->HandleError(memUuidTxt.error(), std::source_location::current());
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
						loadMachineState(onLoad.get());
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
					
					if (m->opt_.RunAsync() == true)
					{
						m->quit_ = true;
					}
					break;
				}
				case ISR::NoInterrupt:
				{
					// no interrupts pending, do any work that is outstanding

					if (m->opt_.RunAsync() == true)
					{
						if (m->quit_ == true)
						{
							quit = true;
						}
					}
					else
					{
						if (m->onIdle_)
						{
							quit = m->onIdle_(m->ioController_.get());
						}
					}
#ifndef ENABLE_MEEN_RP2040
					loadMachineState(checkHandler(onLoad));
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

			return quit;
		};

		m->cpu_->Reset();
		m->clock_->Reset();

		if (m->opt_.ISRFreq() > 0)
		{
			ticksPerIsr = m->clock_->GetSpeed() / m->opt_.ISRFreq();
		}

		auto quit = serviceInterrupts();

		while (quit == false)
		{
			//Execute the next instruction
			ticks = m->cpu_->Execute();
			currTime = m->clock_->Tick(ticks);
			totalTicks += ticks;

			// Check if it is time to service interrupts
			if (totalTicks - lastTicks >= ticksPerIsr || ticks == 0) // when ticks is 0 the cpu is not executing (it has been halted), poll (should be less aggressive) for interrupts to unhalt the cpu
			{
				quit = serviceInterrupts();
				lastTicks = totalTicks;
			}
		}

		m->runTime_ = currTime.count();
	}

	std::expected<uint64_t, std::error_code> Machine::Run()
	{
		if (running_ == true)
		{
			return std::unexpected(HandleError(errc::busy, std::source_location::current()));
		}

		if (memoryController_ == nullptr)
		{
			return std::unexpected(HandleError(errc::memory_controller, std::source_location::current()));
		}

		if (ioController_ == nullptr)
		{
			return std::unexpected(HandleError(errc::io_controller, std::source_location::current()));
		}

		if(clock_ == nullptr)
		{
			return std::unexpected(HandleError(errc::clock_sampling_freq, std::source_location::current()));
		}

		if(cpu_ == nullptr)
		{
			return std::unexpected(HandleError(errc::cpu, std::source_location::current()));
		}

		auto err = clock_->SetSamplingFrequency(opt_.ClockSamplingFreq());

		if (err)
		{
			return std::unexpected(HandleError(err, std::source_location::current()));
		}

		runTime_ = 0;
		running_ = true;
		quit_ = false;

		auto waitForCompletion = [this]
		{
			if(running_ == true)
			{
#ifdef ENABLE_MEEN_RP2040
				auto core1Ret = multicore_fifo_pop_blocking();
	
				if(core1Ret != 0xFFFFFFFF)
				{
					return HandleError(errc::async, std::source_location::current());
				}
#else
				if (fut_.valid() == true)
				{
					fut_.get();
				}
				else
				{
					return HandleError(errc::async, std::source_location::current());
				}
#endif // ENABLE_MEEN_RP2040
				running_ = false;
			}
	
			return std::error_code{};
		};

		auto idleLoop = [this, ioc = ioController_.get()]
		{
			if (onIdle_)
			{
				while (quit_ == false)
				{
					if (onIdle_(ioc) == true)
					{
						// atomic_bool shared with RunMachine thread
						quit_ = true;
					}
				}
			}
		};

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

			// Run an idle loop if an onIdle handler has been registered.
			idleLoop();
			// Wait for the machine to finish
			err = waitForCompletion();

			if (err)
			{
				return std::unexpected(err);
			}
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
		else
		{
			// Run an idle loop if an onIdle handler has been registered.
			idleLoop();
			// Wait for the machine to finish
			err = waitForCompletion();

			if (err)
			{
				return std::unexpected(err);
			}
		}
#endif // ENABLE_MEEN_RP2040
		return runTime_;
	}

	std::error_code Machine::AttachMemoryController (IControllerPtr&& controller)
	{
		if (running_ == true)
		{
			return HandleError(errc::busy, std::source_location::current());
		}

		if(controller == nullptr)
		{
			return HandleError(errc::invalid_argument, std::source_location::current());
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
			return std::unexpected(HandleError(errc::busy, std::source_location::current()));
		}

		if(memoryController_ == nullptr)
		{
			return std::unexpected(HandleError(errc::memory_controller, std::source_location::current()));
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
			return HandleError(errc::busy, std::source_location::current());
		}

		if(controller == nullptr)
		{
			return HandleError(errc::invalid_argument, std::source_location::current());
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
			return std::unexpected(HandleError(errc::busy, std::source_location::current()));
		}

		if(ioController_ == nullptr)
		{
			return std::unexpected(HandleError(errc::io_controller, std::source_location::current()));
		}

		cpu_->SetIoController(nullptr);
		auto controller = std::move(ioController_);
		ioController_ = nullptr;
		return controller;
	}

	std::error_code Machine::OnSave(std::function<errc(const char* json, IController* ioController)>&& onSave)
	{
#ifdef ENABLE_MEEN_SAVE
		if (running_ == true)
		{
			return HandleError(errc::busy, std::source_location::current());
		}

		onSave_ = std::move(onSave);
		return std::error_code{};
#else
		return HandleError(errc::not_implemented, std::source_location::current());
#endif // ENABLE_MEEN_SAVE
	}

	std::error_code Machine::OnLoad(std::function<errc(char* json, int* jsonLen, IController* ioController)>&& onLoad)
	{
		if (running_ == true)
		{
			return HandleError(errc::busy, std::source_location::current());
		}

		onLoad_ = std::move(onLoad);
		return std::error_code{};
	}

	std::error_code Machine::OnIdle(std::function<bool(IController* ioController)>&& onIdle)
	{
		if (running_ == true)
		{
			return HandleError(errc::busy, std::source_location::current());
		}

		onIdle_ = std::move(onIdle);
		return std::error_code{};
	}

	std::error_code Machine::OnError(std::function<void(std::error_code ec, const char* fileName, const char* functionName, uint32_t line, uint32_t column, IController* ioController)>&& onError)
	{
		if (running_ == true)
		{
			return HandleError(errc::busy, std::source_location::current());
		}

		onError_ = std::move(onError);
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