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

#include "meen/utils/ErrorCode.h"

namespace meen
{
	namespace detail
	{
		class category : public std::error_category
		{
		public:
			//cppcheck-suppress unusedFunction
			virtual const char* name() const noexcept final
			{
				return "meen::category";
			}

			//cppcheck-suppress unusedFunction
			virtual std::string message(int ec) const final
			{
				switch(ec)
				{
					case errc::no_error:
						return "Success";
					case errc::busy:
						return "The engine is running";
					case errc::clock_sampling_freq:
						return "The host clock sampling frequency can't be set, either it's too high/low or the host clock can't be queried";
					case errc::cpu:
						return "The cpu is invalid";
					case errc::incompatible_ram:
						return "The ram to load is incompatible with this component";
					case errc::incompatible_rom:
						return "The rom to load is incompatible with this component";
					case errc::incompatible_uuid:
						return "The uuid to load does not match this component";
					case errc::invalid_argument:
						return "An argument supplied to the method is invalid";
					case errc::io_controller:
						return "Invalid io controller";
					case errc::json_config:
						return "A JSON configuration parameter is invalid";
					case errc::json_parse:
						return "A JSON parse error occurred while processing the configuration file/string";
					case errc::memory_controller:
						return "Invalid memory controller";
					case errc::no_zlib:
						return "MEEN compiled without zlib support";
					case errc::not_implemented:
						return "The method is not implemented";
					case errc::unknown_option:
						return "An unknown JSON option was encountered and ignored";
					default:
						return "Unknown error code";
				}
			}
		};
	} // namespace detail

	//cppcheck-suppress unusedFunction
	const std::error_category& category()
	{
		static detail::category instance;
		return instance;
	}
} // namespace meen