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

module;
#include <assert.h>
#include <thread>

module CpuClock;

import <cstdint>;
import <chrono>;
import SystemBus;

using namespace std::chrono;

namespace MachEmu
{
	CpuClock::CpuClock(milliseconds correlateFreq, uint64_t speed) :
		timePeriod_(1000000000 / speed)
	{
		// tick the clock after at least this many ticks
		totalTicks_ = speed / 1000.0 /* millis: 1000 ticks per second*/ * correlateFreq.count();
	}

	nanoseconds CpuClock::Tick(uint64_t ticks)
	{
		if (totalTicks_ >= 0)
		{
			tickCount_ += ticks;

			if (tickCount_ >= totalTicks_)
			{
				auto sleepFor = [this](nanoseconds sleepTime)
				{
					// don't attempt to sleep for anything less than a millisecond 
					if (sleepTime >= nanoseconds(1000000))
					{
						auto now = steady_clock::now();
						std::this_thread::sleep_for(nanoseconds(static_cast<int64_t>(sleepTime.count() * spinPercantageToSleep_)));
						auto total = duration_cast<nanoseconds>(steady_clock::now() - now);
						sleepTime -= total;
					}

					return sleepTime;
				};

				auto spinFor = [this](nanoseconds spinTime)
				{
					auto now = steady_clock::now();

					if (spinTime > nanoseconds::zero())
					{
						auto start = now;
						auto end = (now + spinTime);

						while (now < end)
						{
							now = steady_clock::now();
						}

						// record any over spin
						spinTime = duration_cast<nanoseconds>(end - now);
					}

					return spinTime;
				};


				auto nanos = nanoseconds(tickCount_ * timePeriod_) - duration_cast<nanoseconds>(steady_clock::now() - lastTime_) + error_;
				error_ = spinFor(sleepFor(nanos));
				tickCount_ = 0;
				lastTime_ = steady_clock::now();
				time_ = duration_cast<nanoseconds>(lastTime_ - epoch_);
			}
		}
		else
		{
			lastTime_ = steady_clock::now();
			time_ = duration_cast<nanoseconds>(lastTime_ - epoch_);
		}

		return time_;
	}

	void CpuClock::Reset()
	{
		epoch_ = steady_clock::now();
		lastTime_ = epoch_;
	}
} // namespace MachEmu
