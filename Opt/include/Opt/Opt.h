#ifndef OPT_H
#define OPT_H

#include <string>

namespace MachEmu
{
	/** Machine options

		A wrapper around json option parsing.

		Each public method documents a supported option.
	*/
	class Opt
	{
		private:
			/** Cpu type

				Supported cpus, currently only i8080 is supported.
			*/
			std::string cpuType_{ "i8080" };

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

				@throws		any exception that the underlying json parser can throw, in this case nlohmann_json

				@throws		std::invalid_argument if the interrupt service routine is negative.
			*/
			void SetOptions(const char* json);

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