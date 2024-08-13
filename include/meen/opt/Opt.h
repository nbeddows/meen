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

#ifndef OPT_H
#define OPT_H

#include <string>

#include <nlohmann/json.hpp>
#include <system_error>

namespace MachEmu
{
	/** Machine options

		A wrapper around json option parsing.

		@see	MachEmu::MakeMachine for configuration options.

		@todo	Add option for high resolution timer.
	*/
	class Opt
	{
		private:
			/**
				JSON parser

				An implementation by nlohmann.
			*/
			nlohmann::json json_{};

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
							unknown_option: all recognised options were set successfully though unrecognised options were found.

				@throws		std::runtime_error or any exception that the underlying json parser can throw, in this case nlohmann_json

				@throws		std::runtime_error if the cpu option is specified.

				@throws		std::invalid_argument if the interrupt service routine frequency is negative.
			*/
			std::error_code SetOptions(const char* json);

			/**	Clock resolution

				The frequency at which the internal clock ticks.
			*/
			int64_t ClockResolution() const;

			/** Compressor

				Supported compressors, currently only zlib is supported.
			*/
			std::string Compressor() const;

			/** Cpu type

				Supported cpus, currently only i8080 is supported.
			*/
			std::string CpuType() const;

			/** Text to binary encoder

				Supported encoders, currently only base64 is supported.
			*/
			std::string Encoder() const;

			/** Interrupt service routine frequency

				A multipler applied to the machine clock resolution to alter the rate
				at which interrupts are serviced.
			*/
			double ISRFreq() const;

			/** Machine state load mode

				True for asynchronous, false for synchronous.
			*/
			bool LoadAsync() const;

			/** Ram metadata
			
				This vector defines blocks of ram.

				The first pair entry is the offset from the start of memory to the start of the ram block.
				The second pair entry is the size of the ram block.
			*/
			std::vector<std::pair<uint16_t, uint16_t>> Ram() const;

			/** Rom metadata
			
				This vector defines blocks of rom.

				The first pair entry is the offset from the start of memory to the start of the rom block.
				The second pair entry is the size of the rom block.
			*/
			std::vector<std::pair<uint16_t, uint16_t>> Rom() const;

			/** Machine run mode

				True for asynchronous, false for synchronous.
			*/
			bool RunAsync() const;

			/** Machine state save mode

				True for asynchronous, false for synchronous.
			*/
			bool SaveAsync() const;
	};
} // namespace MachEmu

#endif // OPT_H