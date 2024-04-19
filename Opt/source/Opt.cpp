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

#include <fstream>

#include "nlohmann/json.hpp"
#include "Opt/Opt.h"

namespace MachEmu
{
	Opt::Opt()
	{
		json_ = new nlohmann::json();

		if(json_ == nullptr)
		{
			throw std::bad_alloc();
		}

		*json_ = nlohmann::json::parse(Opt::DefaultOpts());
	}

	Opt::~Opt()
	{
		if (json_ != nullptr)
		{
			delete json_;
			json_ = nullptr;
		}
	}

	constexpr std::string Opt::DefaultOpts()
	{
		std::string defaults =	R"({"clockResolution":-1,"compressor":")"
#ifdef ENABLE_ZLIB
								"zlib"
#else
								"none"
#endif
								R"(","encoder":"base64","isrFreq":0,"loadAsync":false,"ramOffset":0,"ramSize":0,"romOffset":0,"romSize":0,"runAsync":false,"saveAsync":false})";
		return defaults;
	}

	ErrorCode Opt::SetOptions(const char* opts)
	{
		nlohmann::json json;
		auto err = ErrorCode::NoError;

		if(opts == nullptr)
		{
			json = nlohmann::json::parse(Opt::DefaultOpts());
		}
		else
		{
			std::string_view jsonStr(opts);

			if (jsonStr.starts_with("file://") == true)
			{
				jsonStr.remove_prefix(strlen("file://"));
				std::ifstream fin(std::string(jsonStr.data(), jsonStr.size()));
				json = nlohmann::json::parse(fin);
			}
			else
			{
				// parse as if raw json
				json = nlohmann::json::parse(std::string(jsonStr.data(), jsonStr.length()));
			}

			if (json_->contains("cpu") == true && json.contains("cpu") == true)
			{
				throw std::runtime_error("cpu type has already been set");
			}

			if (json.contains("isrFreq") == true && json["isrFreq"].get<double>() < 0)
			{
				throw std::invalid_argument("isrFreq must be >= 0");
			}

#ifndef ENABLE_ZLIB
			if (json.contains("compressor") == true && json["compressor"].get<std::string>() == "zlib")
			{
				throw std::runtime_error("mach-emu has been compiled with no zlib support");
			}
#endif
		}

		json_->update(json);

		return err;
	}

	int64_t Opt::ClockResolution() const
	{
		return (*json_)["clockResolution"].get<int64_t>();
	}

	std::string Opt::Compressor() const
	{
		return (*json_)["compressor"].get<std::string>();
	}

	std::string Opt::CpuType() const
	{
		if (json_->contains("cpu") == true)
		{
			return (*json_)["cpu"].get<std::string>();
		}
		else
		{
			return "";
		}
	}

	std::string Opt::Encoder() const
	{
		return (*json_)["encoder"].get<std::string>();
	}

	double Opt::ISRFreq() const
	{
		return (*json_)["isrFreq"].get<double>();
	}

	bool Opt::LoadAsync() const
	{
		return (*json_)["loadAsync"].get<bool>();
	}

	uint16_t Opt::RamOffset() const
	{
		return (*json_)["ramOffset"].get<uint16_t>();
	}

	uint16_t Opt::RamSize() const
	{
		return (*json_)["ramSize"].get<uint16_t>();
	}

	uint16_t Opt::RomOffset() const
	{
		return (*json_)["romOffset"].get<uint16_t>();
	}

	uint16_t Opt::RomSize() const
	{
		return (*json_)["romSize"].get<uint16_t>();
	}

	bool Opt::RunAsync() const
	{
		return (*json_)["runAsync"].get<bool>();
	}

	bool Opt::SaveAsync() const
	{
		return (*json_)["saveAsync"].get<bool>();
	}
} // namespace MachEmu