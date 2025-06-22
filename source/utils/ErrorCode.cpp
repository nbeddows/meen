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
						return "success";
					case errc::async:
						return "an asynchronous operation failed to complete";
					case errc::busy:
						return "the engine is running";
					case errc::clock_sampling_freq:
						return "the host clock sampling frequency can't be set, either it's too high/low or the host clock can't be queried";
					case errc::cpu:
						return "the cpu is invalid";
					case errc::encoder:
						return "the binary to text encoder is unknown";
					case errc::incompatible_ram:
						return "the ram to load is incompatible with this component";
					case errc::incompatible_rom:
						return "the rom to load is incompatible with this component";
					case errc::incompatible_uuid:
						return "the uuid to load does not match this component";
					case errc::invalid_argument:
						return "an argument supplied to the method is invalid";
					case errc::io_controller:
						return "invalid io controller";
					case errc::json_config:
						return "a JSON configuration parameter is invalid";
					case errc::json_parse:
						return "a JSON parse error occurred while processing the configuration file/string";
					case errc::memory_controller:
						return "invalid memory controller";
					case errc::compressor:
						return "the compressor is unknown";
					case errc::not_implemented:
						return "the method is not implemented";
					case errc::unknown_option:
						return "an unknown JSON option was encountered and ignored";
					case errc::uri_scheme:
						return "the uri scheme is not supported";
					default:
						return "unknown error code";
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