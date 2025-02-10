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

#ifndef OPT_H
#define OPT_H

#ifdef ENABLE_NLOHMANN_JSON
	#include <nlohmann/json.hpp>
#else
	#include <ArduinoJson.h>
#endif
#include <string>
#include <system_error>
#include <vector>

namespace meen
{
	/** Machine options

		A wrapper around json option parsing.

		@see	meen::MakeMachine for configuration options.

		@todo	Add option for high resolution timer.
	*/
	class Opt
	{
		private:
#ifdef ENABLE_NLOHMANN_JSON
			/**
				JSON parser

				An implementation by nlohmann.
			*/
			nlohmann::json json_{};
#else
			JsonDocument json_{};

			static void Merge(JsonVariant dst, JsonVariantConst src);
#endif
			/**
				Default options

				The default options to use when nullptr is passed to SetOptions
			*/
			static constexpr std::string DefaultOpts();

		public:
			Opt();
			~Opt() = default;
			/** Update machine options

				Process a json string view for supported options.

				@param		opts	A json string specifying the desired options to update. Passing in a json string of nullptr will set all options
									to their defaults.

				@return		no_error: all options were set successfully.<br>
							json_parse: the json input is malformed.<br>
							json_config: the isr frequency is negative.<br>
							no_zlib: zib option was specifed but zlib has been disabled.
			*/
			std::error_code SetOptions(const char* json);

			/**	Clock resolution

				The frequency at which the internal clock ticks.
			*/
			int64_t ClockResolution() const;

			/** Interrupt service routine frequency

				A multipler applied to the machine clock resolution to alter the rate
				at which interrupts are serviced.
			*/
			double ISRFreq() const;

			/** Machine run mode

				True for asynchronous, false for synchronous.
			*/
			bool RunAsync() const;

			/** Compressor

				Supported compressors, currently only zlib is supported.
			*/
			std::string Compressor() const;

			/** Text to binary encoder

				Supported encoders, currently only base64 is supported.
			*/
			std::string Encoder() const;

			/** Machine state load mode

				True for asynchronous, false for synchronous.
			*/
			bool LoadAsync() const;

#ifdef ENABLE_MEEN_SAVE
			/** Machine state save mode

				True for asynchronous, false for synchronous.
			*/
			bool SaveAsync() const;
#endif
	};
} // namespace meen

#endif // OPT_H