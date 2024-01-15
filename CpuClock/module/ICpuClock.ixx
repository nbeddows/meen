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
		/** Tick.
		
			Advances the cpu by the specified number of ticks.

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

		/** Reset.

			Resets the epoch of the clock.
		*/
		virtual void Reset() = 0;

		virtual ~ICpuClock() = default;
	};
} // namespace MachEmu