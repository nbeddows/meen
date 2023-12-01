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

module CpuClock;

using namespace std::chrono;

constexpr bool dbgClock = false;

namespace Emulator
{
	CpuClock::CpuClock(nanoseconds timePeriod, nanoseconds correlateFreq) :
		timePeriod_(timePeriod),
		correlateFreq_(correlateFreq)
	{
		tp_ = steady_clock::now();
	}

	CpuClock::CpuClock(std::shared_ptr<ControlBus<8>> controlBus, nanoseconds timePeriod) :
		controlBus_(controlBus),
		timePeriod_(timePeriod)
	{
		epoch_ = steady_clock::now();
		tp_ = epoch_;
	}

	nanoseconds CpuClock::TimePeriod() const
	{
		return timePeriod_;
	}

	nanoseconds CpuClock::CorrelateFrequency() const
	{
		return correlateFreq_;
	}

	void CpuClock::Reset()
	{
		elapsedTime_ = nanoseconds::zero();
		hostNanos_ = nanoseconds::zero();
		targetNanos_ = nanoseconds::zero();
		epoch_ = steady_clock::now();
		tp_ = epoch_;
	}

	nanoseconds CpuClock::Tick()
	{
		auto now = steady_clock::now();

		if (now >= tp_ + timePeriod_)
		{
			controlBus_->Send (Signal::Clock);
			tp_ = now;
		}

		return now - epoch_;
	}

	#if 0
	nanoseconds CpuClock::Tick() const
	{
		auto sleepTime = nanoseconds::zero();
		auto startClock = steady_clock::now();
		auto currentClock = startClock;
		
		auto targetSpinTime = timePeriod_;

		if (targetSpinTime > minSpinTimeForSleep_)
		{
			auto mostOfTheSpin = static_cast<uint64_t>(targetSpinTime.count() * spinPercantageToSleep_);

			std::this_thread::sleep_for(nanoseconds(mostOfTheSpin));
			sleepTime = std::chrono::steady_clock::now() - currentClock;
			currentClock += sleepTime;
			targetSpinTime -= sleepTime;

			if constexpr (dbgClock == true)
			{
				//we slept longer than the target ... it should compensate for this, but
				//if it happens a lot we can reduce the spinPercantageToSleep_
				if (targetSpinTime < nanoseconds::zero())
				{
					printf("Over slept by %I64d nanos\n", abs(targetSpinTime.count()));
				}
			}
		}
		//else if (targetSpinTime > 5)
		//{
		//	while (currentClock < targetClock)
		//	{
		//		std::this_thread::sleep_for(nanoseconds(1));
		//		currentClock = steady_clock::now();
		//	}
		//}

		//auto spinStart = currentClock;
		//auto targetClock = spinStart + targetSpinTime;
		auto targetClock = currentClock + targetSpinTime;

		while (currentClock < targetClock)
		{
			std::this_thread::sleep_for(nanoseconds(0));
			currentClock = steady_clock::now();
		}

		if constexpr (dbgClock == true)
		{
			if (targetSpinTime > nanoseconds::zero() && currentClock > targetClock)
			{
				printf("Over spun by %I64d nanos\n", (currentClock - targetClock).count());
			}
		}

		return currentClock - startClock;
	}
	#endif

	nanoseconds CpuClock::Tick(uint16_t ticks)
	{
		auto throttleTime = nanoseconds::zero();
		auto elapsed = timePeriod_ * ticks;
		targetNanos_ += elapsed;
		elapsedTime_ += elapsed;
		auto now = steady_clock::now();
		hostNanos_ += (now - tp_);

		if (elapsedTime_ >= correlateFreq_)
		{
			auto targetSpinTime = (targetNanos_ - hostNanos_);

			if (targetSpinTime > nanoseconds::zero())
			{
				//the host CPU exectued faster than the target,
				//we need to slow down the host(sleep/spin)

				/*
					If the target spin time is greater than 50ms (minSpinTimeForSleep_), we will
					sleep for 80% (spinPercantageToSleep_) of the spin time, then spin away the rest.
				*/
				auto sleepTime = nanoseconds::zero();
				auto currentClock = steady_clock::now();

				if (targetSpinTime >= minSpinTimeForSleep_)
				{
					auto mostOfTheSpin = static_cast<uint64_t>(targetSpinTime.count() * spinPercantageToSleep_);
					
					std::this_thread::sleep_for(nanoseconds(mostOfTheSpin));
					sleepTime = std::chrono::steady_clock::now() - currentClock;
					currentClock += sleepTime;
					targetSpinTime -= sleepTime;
					
					if constexpr (dbgClock == true)
					{
						//we slept longer than the target ... it should compensate for this, but
						//if it happens a lot we can reduce the spinPercantageToSleep_
						if (targetSpinTime < nanoseconds::zero())
						{
							//cppcheck-suppress invalidPrintfArgType_sint
							printf("Over slept by %lld nanos\n", abs(targetSpinTime.count()));
						}
					}
				}

				auto spinStart = currentClock;
				auto targetClock = spinStart + targetSpinTime;

				while (currentClock < targetClock)
				{
					//std::this_thread::sleep_for(nanoseconds(1));
					currentClock = steady_clock::now();
				}

				if constexpr (dbgClock == true)
				{
					if (targetSpinTime > nanoseconds::zero() && currentClock > targetClock)
					{
						printf ("Over spun by %I64d nanos\n", (currentClock - targetClock).count());
					}
				}

				//throttle time = sleepTime + how much we actually spun
				throttleTime = sleepTime + (currentClock - spinStart);

				//This essentially stores the error (0 - we throttled the exact amount, > 0 throttled too much, < 0 throttled too little
				//targetSpinTime -= throttleTime;
			}
			//else
			//{
			//	if constexpr (dbgClock == true)
			//	{
			//		printf("Host CPU behind by %I64d nanos\n", abs(targetSpinTime.count()));
			//	}
			//}

			//reset the elapsed time for the next correlate check
			elapsedTime_ = nanoseconds::zero();
		}

		tp_ = now;
		return hostNanos_ + throttleTime;
	}

	nanoseconds CpuClock::Time() const
	{
		return targetNanos_;
	}
}
