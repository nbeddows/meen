#ifndef OPT_H
#define OPT_H

#include <string>

#include "Base/Base.h"

namespace MachEmu
{
	/** Machine options

		A wrapper around json option parsing.

		Each public method documents a supported option.

		@todo	Add option for high resolution timer.
	*/
	class Opt
	{
		private:
			/** Cpu type

				Supported cpus, currently only i8080 is supported.
			*/
			std::string cpuType_{};

			/** Interrupt service routine frequency

				A multipler applied to the machine clock resolution to alter the rate
				at which interrupts are serviced.
			*/
			double isrFreq_{};

			/** Machine run mode

				True for asynchronous, false for synchronous.
			*/
			bool runAsync_{};

		public:
			/** Update machine options

				Process a json string view for supported options.

				@param		opts	A json string specifying the desired options to update. Passing in a json string of nullptr will set all options
									to their defaults.

				@return		ErrorCode::NoError: all options were set successfully.
							ErrorCode::UnknownOption: all recognised options were set successfully though unrecognised options were found.

				@throws		any exception that the underlying json parser can throw, in this case nlohmann_json

				@throws		std::runtime_error if the cpu option is specified.

				@throws		std::invalid_argument if the interrupt service routine frequency is negative.
			*/
			ErrorCode SetOptions(const char* json);

			/** Cpu type

				@see cpuType_
			*/
			const char* CpuType() const;

			/** Interrupt service routine frequency

				@see IsrFreq_
			*/
			double ISRFreq() const;

			/** Run asynchronous

				@see runAsync_
			*/
			bool RunAsync() const;
	};
} // namespace MachEmu

#endif // OPT_H