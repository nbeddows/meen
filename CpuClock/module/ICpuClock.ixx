export module ICpuClock;

import <chrono>;
import <cstdlib>;
import SystemBus;

using namespace std::chrono;

namespace Emulator
{
	/** ICpuClock
	
		Represents the clock of an emulated cpu.

	*/
	export struct ICpuClock
	{
		/** TimePeriod
			
			Returns in nanoseconds the duration of a clock cycle (tick).

			@return		nanoseconds		The tick duration.
		*/
		virtual nanoseconds TimePeriod() const = 0;

		/** CorrelateFrequency
			
			The frequency in nanoseconds at which the clock will syncthe target cpu
			to the correct rate.

			@discussion		A frequency of zero will sync the cpu at every call to Tick().
							The lower the frequency the more often the cpu will spin to maintain sync
							(most accurate, more cpu usage, less jitter), anything above 50,000,000 nanos
							(50 milliseconds) will sleep the cpu thread to maintain sync
							(less accurate, less cpu usage, more jitter).

			@return			nanoseconds		The frequency at which the target cpu will correlate.
		*/
		virtual nanoseconds CorrelateFrequency() const = 0;
		
		/** Reset
			
			Resets the cpu to its initial state when forst created.
		*/
		virtual void Reset() = 0;
		
		/** Tick
		
			Advances the cpu by the specified number of ticks.

			@discussion		The tick is the smallest unit at which the cpu
							can advance. It can only move forward hence the
							parameter is unsigned. Therefore the smallest
							interval that the cpu can be advanced is a single
							time period.

							nanosecondsToAdvance = ticks * TimePeriod

							@see TimePeriod
			
			@return			nanoseconds		The current time of the machine which is
											emulating the target cpu.

											For the most part the return value can be
											ignored, it is primarily used in the tests
											to assert the accuracy of the clock.
		*/
		virtual nanoseconds Tick(uint16_t ticks) = 0;
		//virtual nanoseconds Tick() const = 0;
		virtual nanoseconds Tick() = 0;
		
		/** Time
		
			The current time in nanoseconds of the emulated cpu

			@return		nanoseconds		The current emulated cpu time.
		*/
		virtual nanoseconds Time() const = 0;

		virtual ~ICpuClock() = default;
	};

	/* THIS NEEDS TO BE MOVED TO CpuClockFactory.ixx*/
	
	/** Factory function to make a cpu clock
	
		The CPU clock contols the timing for the target CPU so that all instructions executed
		execute at the correct rate.

		@param	timePeriod				The duration in nanoseconds of the target cpu tick (cycle).
		
		@param	correlateFrequency		The frequency in nanoseconds at which the clock will sync
										The target cpu to the correct rate.

										@see ICpuClock::CorrelateFrequency
										@see ICpuClock::Tick

		@return	unique_ptr				A unique_ptr to the CpuClock interface.
	*/
	export std::unique_ptr<ICpuClock> MakeCpuClock(nanoseconds timePeriod, nanoseconds correlateFrequency);
	//export template <uint8_t crtlW>
	export std::unique_ptr<ICpuClock> MakeCpuClock(std::shared_ptr<ControlBus<8>> controlBus, nanoseconds timePeriod);
}