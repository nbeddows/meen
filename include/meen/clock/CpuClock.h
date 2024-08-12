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

#ifndef CPUCLOCK_H
#define CPUCLOCK_H

#include <cstdint>
#include <chrono>
#include <memory>

#include "meen/clock/ICpuClock.h"

namespace MachEmu
{
	//Slow down the current thread to emulate the desired clock speed.
	//Achieved through a combination of sleep/spin.
	class CpuClock final : public ICpuClock
	{
	private:
		//Since sleep only guarantees a minimim sleep time, asking for
		//what we really want will more than likley mean that we will
		//over sleep, so we only ask for a percentage of what we want,
		//(this can be reduced if we continually oversleep) then spin
		//for the remainder.
		//cppcheck-suppress unusedStructMember
		static constexpr double spinPercantageToSleep_{ 0.7 };
		// The number of ticks to accumulate before a correlation occurs.
		// Set the default to -1 (don't sync the clock, run as fast as possible)
		//cppcheck-suppress unusedStructMember
		int totalTicks_{-1};
		// the current tick count in this correlation period.
		//cppcheck-suppress unusedStructMember
		int tickCount_{};
		//The amount of time for one cycle to complete,
		//this tends to be variable, for example, on the
		//i8080 it ranges for 480ns to 2000ns.
		//Could use a range here and then use a random
		//value within the range .... or not ....
		//just leaving it constant.
		std::chrono::nanoseconds timePeriod_{};
		// the total amount of oversleep
		std::chrono::nanoseconds error_{};
		//The time at which this clock begun.
		std::chrono::steady_clock::time_point epoch_{};
		//The time at which this clock was sampled.
		std::chrono::steady_clock::time_point lastTime_{};
		// the current time of the clock expressed at a frequency as specified by correlateFreq
		std::chrono::nanoseconds time_{};
		// the maximum resolution of the host clock
		std::chrono::nanoseconds maxResolution_{};

	public:
		//correlateFreq
		//The interval at which to correlate this clock with the steady_clock (in nanos).
		//0  - correlate at every 'Tick' call,
		//-1 - do not correlate (run as fast as possible)
		//correlating at every tick or close to it will force a spin to maintain
		//sync, anything above 50ms will allow a sleep for part of the time at the
		//expense of sync accuracy.
		CpuClock(uint64_t speed);
		~CpuClock() = default;

		void Reset() final;
		std::error_code SetTickResolution(std::chrono::nanoseconds resolution, int64_t* resolutionInTicks) final;

		//Returns the host CPU time.
		std::chrono::nanoseconds Tick(uint64_t ticks) final;
	};
} // namespace MachEmu

#endif // CPUCLOCK_H