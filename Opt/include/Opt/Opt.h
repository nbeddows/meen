#ifndef OPT_H
#define OPT_H

#include <string>

#include "Base/Base.h"
#include "nlohmann/json_fwd.hpp"

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
				json parser

				an implementation by nlohmann
			*/
			nlohmann::json* json_{};

		public:
			Opt();
			~Opt();
			/** Update machine options

				Process a json string view for supported options.

				@param		opts	A json string specifying the desired options to update. Passing in a json string of nullptr will set all options
									to their defaults.

				@return		ErrorCode::NoError: all options were set successfully.<br>
							ErrorCode::UnknownOption: all recognised options were set successfully though unrecognised options were found.

				@throws		std::runtime_error or any exception that the underlying json parser can throw, in this case nlohmann_json

				@throws		std::runtime_error if the cpu option is specified.

				@throws		std::invalid_argument if the interrupt service routine frequency is negative.
			*/
			ErrorCode SetOptions(const char* json);

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

			/** Machine run mode

				True for asynchronous, false for synchronous.
			*/
			bool RunAsync() const;
	};
} // namespace MachEmu

#endif // OPT_H