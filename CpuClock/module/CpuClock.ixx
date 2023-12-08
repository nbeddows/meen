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

export module CpuClock;

import <chrono>;
import <memory>;
import <thread>;
import ICpuClock;
import SystemBus;

using namespace std::chrono;

namespace MachEmu
{
	//Slow down the current thread to emulate the desired clock speed.
	//Achieved through a combination of sleep/spin.
	export class CpuClock final : public ICpuClock
	{
	private:
		//The minimum spin time that must be accumulated before a sleep can occur.
		//Due to O/S scheduling it really can't be less than this.
		static constexpr nanoseconds minSpinTimeForSleep_{ 50000000 };

		//Since sleep only guarantees a minimim sleep time, asking for
		//what we really want will more than likley mean that we will
		//over sleep, so we only ask for a percentage of what we want,
		//(this can be reduced if we continually oversleep) then spin
		//for the remainder.
		//cppcheck-suppress unusedStructMember
		static constexpr double spinPercantageToSleep_{ 0.8 };

		//Used to signal a tick of the clock
		std::shared_ptr<ControlBus<8>> controlBus_;
		
		//The amount of time for one cycle to complete,
		//this tends to be variable, for example, on the
		//i8080 it ranges for 480ns to 2000ns.
		//Could use a range here and then use a random
		//value within the range .... or not ....
		//just leaving it constant.
		nanoseconds timePeriod_{};

		//The interval at which to correlate this clock with the steady_clock (in nanos).
		//0 - correlate at every 'Tick' call,
		//correlating at every tick or close to it will force a spin to maintain
		//sync, anything above 50ms will allow a sleep for part of the time at the
		//expense of sync accuracy.
		nanoseconds correlateFreq_{};

		//When the elapsedTime_ >= correlateFreq_ then perform sleep and/or spin.
		nanoseconds elapsedTime_{};		

		//Target and host CPU elapsed nanos.
		nanoseconds targetNanos_{};
		nanoseconds hostNanos_{};

		//Current host CPU time.
		steady_clock::time_point tp_{};

		//The time at which this clock begun.
		steady_clock::time_point epoch_{};
	public:
		CpuClock(nanoseconds timePeriod, nanoseconds correlateFreq);
		CpuClock(std::shared_ptr<ControlBus<8>> controlBus, nanoseconds timePeriod);

		//Return the time period of the cpu.
		nanoseconds TimePeriod() const override final;
		//Return the frequency at which to sync the
		//target cpu with the host.
		nanoseconds CorrelateFrequency() const override final;

		//Reset the clock.
		void Reset() override final;

		//Returns the host CPU time.
		nanoseconds Tick(uint16_t ticks) override final;
		//Returns the time of an actual tick.
		//Should be close to timePeriod_ as possible
		//nanoseconds Tick() const;
		nanoseconds Tick() override final;

		//Returns the target CPU time.
		nanoseconds Time() const override final;
	};
} // namespace MachEmu
