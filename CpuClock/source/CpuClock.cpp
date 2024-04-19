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

#ifdef __GNUC__
// use nanosleep
#include <time.h>
#elif defined _WINDOWS
// use hi-res window sleep
#include <Windows.h>
#else
// use std::thread::sleep_for
#include <thread>
#endif

#include "CpuClock/CpuClock.h"

using namespace std::chrono;

namespace MachEmu
{
	CpuClock::CpuClock(uint64_t speed) :
		timePeriod_(1000000000 / speed)
	{
		// tick the clock after at least this many ticks
		//totalTicks_ = speed / 1000.0 /* millis: 1000 ticks per second*/ * correlateFreq.count();
#ifdef __GNUC__
		struct timespec res;
		auto err = clock_getres(CLOCK_REALTIME, &res);

		if (err < 0)
		{
			throw std::runtime_error("Failed to query clock resolution");
		}

		maxResolution_ = nanoseconds(res.tv_nsec);

#elif _WINDOWS
		auto ntdll = LoadLibrary("ntdll.dll");
		
		if (ntdll == nullptr)
		{
			throw std::runtime_error("Failed to query clock resolution");
		}
		
		typedef long(NTAPI* pNtQueryTimerResolution)(unsigned long* MinimumResolution, unsigned long* MaximumResolution, unsigned long* CurrentResolution);
		typedef long(NTAPI* pNtSetTimerResolution)(unsigned long RequestedResolution, char SetResolution, unsigned long* ActualResolution);

		pNtQueryTimerResolution NtQueryTimerResolution = (pNtQueryTimerResolution)GetProcAddress(ntdll, "NtQueryTimerResolution");
		pNtSetTimerResolution   NtSetTimerResolution = (pNtSetTimerResolution)GetProcAddress(ntdll, "NtSetTimerResolution");
		
		if (NtQueryTimerResolution == nullptr || NtSetTimerResolution == nullptr)
		{
			FreeLibrary(ntdll);
			throw std::runtime_error("Failed to query clock resolution");
		}

		// Query for the highest accuracy timer resolution.
		unsigned long minimum, maximum, current;
		NtQueryTimerResolution(&minimum, &maximum, &current);
		// Set the timer resolution to the highest - if we do set it we MUST call it again with FALSE
		// when we exit to restore the system default.
		//NtSetTimerResolution(maximum, TRUE, &current);
		// This is the resolution that the high resolution timer should run at
		maxResolution_ = nanoseconds(maximum * 100);
		// We are done
		FreeLibrary(ntdll);

#else
		maxResolution_ = nanoseconds(40000000);
#endif
	}

	ErrorCode CpuClock::SetTickResolution(std::chrono::nanoseconds resolution, int64_t* resolutionInTicks)
	{
		auto err = ErrorCode::NoError;

		if (resolution >= nanoseconds::zero() && timePeriod_ >= nanoseconds::zero())
		{
			if (resolution < maxResolution_)
			{
				err = ErrorCode::ClockResolution;
			}

			totalTicks_ = resolution / timePeriod_;
		}
		else
		{
			totalTicks_ = -1;
		}

		if (resolutionInTicks != nullptr)
		{
			*resolutionInTicks = totalTicks_;
		}

		return err;
	}

	nanoseconds CpuClock::Tick(uint64_t ticks)
	{
		if (totalTicks_ >= 0)
		{
			tickCount_ += ticks;

			if (tickCount_ >= totalTicks_)
			{
				auto sleepFor = [this](nanoseconds spinTime)
				{
					// don't attempt to sleep for anything less than a millisecond 
					if (spinTime >= nanoseconds(1000000))
					{
						auto sleepTime = static_cast<int64_t>(spinTime.count() * spinPercantageToSleep_);
						auto now = steady_clock::now();
#ifdef __GNUC__
						struct timespec req{ 0, sleepTime };
						nanosleep(&req, nullptr);
#elif defined _WINDOWS
						LARGE_INTEGER sleepPeriod;
						// Convert from nanoseconds to 100 nanosescond units, and negative for relative time.
						sleepPeriod.QuadPart = -(sleepTime / 100);

						// Create the timer, sleep until time has passed, and clean up - available since the 1803 version of Windows 10.
						// Sleep down to 0.5 ms intervals without raising the system level interrupt frequency, which is much friendlier.
						HANDLE timer = CreateWaitableTimerEx(nullptr, nullptr, CREATE_WAITABLE_TIMER_HIGH_RESOLUTION, TIMER_ALL_ACCESS);
						SetWaitableTimer(timer, &sleepPeriod, 0, nullptr, nullptr, 0);
						WaitForSingleObject(timer, INFINITE);
						CloseHandle(timer);
#else
						std::this_thread::sleep_for(nanoseconds(sleepTime));
#endif
						spinTime -= duration_cast<nanoseconds>(steady_clock::now() - now);
					}

					return spinTime;
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
