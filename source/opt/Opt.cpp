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
#include <fstream>

#include "meen/opt/Opt.h"
#include "meen/utils/ErrorCode.h"

namespace meen
{
#ifndef ENABLE_NLOHMANN_JSON
	void Opt::Merge(JsonVariant dst, JsonVariantConst src)
	{
		if(src.is<JsonObjectConst>())
		{
			for(JsonPairConst kvp : src.as<JsonObjectConst>())
			{
				if(dst[kvp.key()])
				{
					Opt::Merge(dst[kvp.key()], kvp.value());
				}
				else
				{
					dst[kvp.key()] = kvp.value();
				}
			}
		}
		else
		{
			dst.set(src);
		}
	};
#endif // ENABLE_NLOHMANN_JSON

	Opt::Opt()
	{
#ifdef ENABLE_NLOHMANN_JSON
		json_ = nlohmann::json::parse(Opt::DefaultOpts(), nullptr, false);
		assert(json_.is_discarded() == false);
#else
		auto e = deserializeJson(json_, Opt::DefaultOpts());
		assert(!e);
#endif // ENABLE_NLOHMANN_JSON
	}

	constexpr std::string Opt::DefaultOpts()
	{
		std::string defaults =	R"({"clockSamplingFreq":-1)"
								R"(,"compressor":")"
#ifdef ENABLE_ZLIB
								"zlib"
#else
								"none"
#endif // ENABLE_ZLIB
								R"(","encoder":")"
#ifdef ENABLE_BASE64
								"base64"
#else
								"none"
#endif // ENABLE_BASE64
#ifdef ENABLE_MEEN_SAVE
								R"(","loadAsync":false,"saveAsync":false)"
#else
								R"(")"
#endif // ENABLE_MEEN_SAVE
								R"(,"isrFreq":0,"runAsync":false})";
		return defaults;
	}

	std::error_code Opt::SetOptions(const char* opts)
	{
		auto err = make_error_code(errc::no_error);
#ifdef ENABLE_NLOHMANN_JSON
		nlohmann::json json;
#else
		JsonDocument json;
#endif // ENABLE_NLOHMANN_JSON

		if(opts == nullptr)
		{
#ifdef ENABLE_NLOHMANN_JSON
			json = nlohmann::json::parse(Opt::DefaultOpts(), nullptr, false);
			assert(json.is_discarded() == false);

			if(json.is_discarded() == true)
			{
				return make_error_code(errc::json_parse);
			}
#else
			auto e = deserializeJson(json, Opt::DefaultOpts());
			assert(!e);

			if(e)
			{
				return make_error_code(errc::json_parse);
			}
#endif // ENABLE_NLOHMANN_JSON
		}
		else
		{
			std::string_view jsonStr(opts);

			if (jsonStr.starts_with("file://") == true)
			{
				jsonStr.remove_prefix(strlen("file://"));
				//todo: use FILE* here, test wih nullptr, make sure it doesn't throw
				std::ifstream fin(std::string(jsonStr.data(), jsonStr.size()));
#ifdef ENABLE_NLOHMANN_JSON
				json = nlohmann::json::parse(fin, nullptr, false);
				if(json.is_discarded() == true)
				{
					return make_error_code(errc::json_parse);
				}
#else
				auto e = deserializeJson(json, fin);
				assert(e);

				if(e)
				{
					return make_error_code(errc::json_parse);
				}
#endif // ENABLE_NLOHMANN_JSON
			}
			else if (jsonStr.starts_with("json://") == true)
			{
				jsonStr.remove_prefix(strlen("json://"));
#ifdef ENABLE_NLOHMANN_JSON
				json = nlohmann::json::parse(std::string(jsonStr.data(), jsonStr.length()), nullptr, false);

				if(json.is_discarded() == true)
				{
					return make_error_code(errc::json_parse);
				}
#else
				auto e = deserializeJson(json, jsonStr.data(), jsonStr.length());
				assert(!e);

				if(e)
				{
					return make_error_code(errc::json_parse);
				}
#endif // ENABLE_NLOHMANN_JSON
			}
			else
			{
				return make_error_code(errc::json_config);
			}
#ifdef ENABLE_NLOHMANN_JSON
			if (json.contains("isrFreq") == true && json["isrFreq"].get<double>() < 0)
#else
			if (json["isrFreq"] != nullptr && json["isrFreq"].as<double>() < 0)
#endif // ENABLE_NLOHMANN_JSON
			{
				return make_error_code(errc::json_config);
			}
#ifndef ENABLE_ZLIB
#ifdef ENABLE_NLOHMANN_JSON
			if (json.contains("compressor") == true && json["compressor"].get<std::string_view>() == "zlib")
#else
			if (json["compressor"] != nullptr && json["compressor"].as<std::string_view>() == "zlib")
#endif // ENABLE_NLOHMANN_JSON
			{
				return make_error_code(errc::no_zlib);
			}
#endif // ENABLE_ZLIB
		}

#ifdef ENABLE_NLOHMANN_JSON
		json_.update(json);
#else
		Opt::Merge(json_, json);
#endif // ENABLE_NLOHMANN_JSON
		return err;
	}

	double Opt::ClockSamplingFreq() const
	{
		#ifdef ENABLE_NLOHMANN_JSON
				return json_["clockSamplingFreq"].get<int64_t>();
		#else
				return json_["clockSamplingFreq"].as<int64_t>();
		#endif // ENABLE_NLOHMANN_JSON
	}

	double Opt::ISRFreq() const
	{
#ifdef ENABLE_NLOHMANN_JSON
		return json_["isrFreq"].get<double>();
#else
		return json_["isrFreq"].as<double>();
#endif // ENABLE_NLOHMANN_JSON
	}

	bool Opt::RunAsync() const
	{
#ifdef ENABLE_NLOHMANN_JSON
		return json_["runAsync"].get<bool>();
#else
		return json_["runAsync"].as<bool>();
#endif // ENABLE_NLOHMANN_JSON
	}

	std::string Opt::Compressor() const
	{
#ifdef ENABLE_NLOHMANN_JSON
		return json_["compressor"].get<std::string>();
#else
		return json_["compressor"].as<std::string>();
#endif // ENABLE_NLOHMANN_JSON
	}

	std::string Opt::Encoder() const
	{
#ifdef ENABLE_NLOHMANN_JSON
		return json_["encoder"].get<std::string>();
#else
		return json_["encoder"].as<std::string>();
#endif // ENABLE_NLOHMANN_JSON
	}

	bool Opt::LoadAsync() const
	{
#ifdef ENABLE_MEEN_SAVE
#ifdef ENABLE_NLOHMANN_JSON
		return json_["loadAsync"].get<bool>();
#else
		return json_["loadAsync"].as<bool>();
#endif // ENABLE_NLOHMANN_JSON
#else
		return false;
#endif // ENABLE_MEEN_SAVE
	}

#ifdef ENABLE_MEEN_SAVE
	bool Opt::SaveAsync() const
	{
#ifdef ENABLE_NLOHMANN_JSON
		return json_["saveAsync"].get<bool>();
#else
		return json_["saveAsync"].as<bool>();
#endif // ENABLE_NLOHMANN_JSON
	}
#endif // ENABLE_MEEN_SAVE
} // namespace meen