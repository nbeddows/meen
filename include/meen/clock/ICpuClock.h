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

#ifndef ICPUCLOCK_H
#define ICPUCLOCK_H

#include <chrono>
#include <cstdint>

#include <system_error>

namespace meen
{
	/** ICpuClock
	
		Represents the clock of an emulated cpu.
	*/
	struct ICpuClock
	{		
		/** Tick.
		
			Advances the clock by the specified number of ticks.

			@discussion		The tick is the smallest unit at which the cpu
							can advance. It can only move forward hence the
							parameter is unsigned. Therefore the smallest
							interval that the cpu can be advanced is a single
							time period.

							nanosecondsToAdvance = ticks * TimePeriod

							@see TimePeriod
			
			@param			ticks			The number of ticks to advance the clock.
			
			@return			nanoseconds		The current time of the machine which is
											emulating the target cpu.
		*/
		virtual std::chrono::nanoseconds Tick(uint64_t ticks) = 0;

		/** Set the resolution of the ticking clock.
		
			@param	samplingFrequency	The frequency in Hertz at which the clock will sync the target cpu to the correct rate.

										A frequency of 0 will sync the cpu at every call to Tick() (this will spin the host
										cpu 100% of the time to maintain sync (most accurate, high cpu usage)).
										A frequency of -1 will not perform any synchronisation (run as fast as possible).

										The higher the frequency the higher the resolution of the clock (disregarding the
										0 and -1 cases diascussed above).

										High frequencies can yield inaccurate clock speeds when a high
										resolution clock is not available (anything that causes sync to occur less than 30-50 milliseconds).
										When a high resolution clock is available this value would be anything that causes sync to occur
										at intervals of less than 1 millisecond.

										@see ICpuClock::Tick

			@return						no_error			The resolution was set successfully.
										clock_resolution	The resolution was set, however, the host does not support
															a high enough resolution timer for this resolution.
		*/
		virtual std::error_code SetSamplingFrequency(double samplingFrequency) = 0;

		/** Get the speed of the Clock in Hertz
		
			This will be the equal to the speed parameter that was passed to the relevant MakeCpuClock factory method.

			@return 					The speed of the clock in Hertz as a uint64_t.
		*/
		virtual uint64_t GetSpeed() const = 0;

		/** Reset.

			Resets the epoch of the clock.
		*/
		virtual void Reset() = 0;

		virtual ~ICpuClock() = default;
	};
} // namespace meen

#endif // ICPUCLOCK_H