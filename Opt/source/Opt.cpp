#include <fstream>

#include "nlohmann/json.hpp"
#include "Opt/Opt.h"

namespace MachEmu
{
	ErrorCode Opt::SetOptions(const char* opts)
	{
		auto err = ErrorCode::NoError;
		nlohmann::json json;
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

		for (auto it = json.begin(); it != json.end(); ++it)
		{		
			const auto& key = it.key();

			if (key == "cpu")
			{
				if (cpuType_.empty() == true)
				{
					cpuType_ = it.value();
				}
				else
				{
					throw std::runtime_error("cpu type has already been set");
				}
			}
			else if (key == "isrFreq")
			{
				isrFreq_ = it.value();

				if (isrFreq_ < 0)
				{
					throw std::invalid_argument("isrFreq must be >= 0");
				}
			}
			else if (key == "runAsync")
			{
				runAsync_ = it.value();
#ifndef _WINDOWS
				if (runAsync_ == true)
				{
					runAsync_ == false;
					err = ErrorCode::NotImplemented;
				}
#endif
			}
			else
			{
				err = ErrorCode::UnknownOption;
			}
		}

		return err;
	}

	const char* Opt::CpuType() const
	{
		return cpuType_.c_str();
	}

	double Opt::ISRFreq() const
	{
		return isrFreq_;
	}

	bool Opt::RunAsync() const
	{
		return runAsync_;
	}
} // namespace MachEmu
