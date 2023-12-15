/*
Copyright (c) 2021-2023 Nicolas Beddows <nicolas.beddows@gmail.com>

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

export module ICpuClock;

import <chrono>;
import <cstdint>;

namespace MachEmu
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
		virtual std::chrono::nanoseconds TimePeriod() const = 0;

		/** CorrelateFrequency
			
			The frequency in nanoseconds at which the clock will sync the target cpu
			to the correct rate.

			@discussion		A frequency of zero will sync the cpu at every call to Tick().
							The lower the frequency the more often the cpu will spin to maintain sync
							(most accurate, more cpu usage, less jitter), anything above 50,000,000 nanos
							(50 milliseconds) will sleep the cpu thread to maintain sync
							(less accurate, less cpu usage, more jitter).

			@return			nanoseconds		The frequency at which the target cpu will correlate.
		*/
		virtual std::chrono::nanoseconds CorrelateFrequency() const = 0;
		
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
		//virtual std::chrono::nanoseconds Tick(uint16_t ticks) = 0;
		//virtual nanoseconds Tick() const = 0;
		virtual std::chrono::nanoseconds Tick() = 0;
		
		/** Time
		
			The current time in nanoseconds of the emulated cpu

			@return		nanoseconds		The current emulated cpu time.
		*/
		virtual std::chrono::nanoseconds Time() const = 0;

		virtual ~ICpuClock() = default;
	};
} // namespace MachEmu