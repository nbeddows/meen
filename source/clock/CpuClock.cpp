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

#ifndef PICO_BOARD
#ifdef __GNUC__
// use nanosleep
#include <time.h>
#elif defined _WINDOWS
// use hi-res window sleep
#include <Windows.h>
#else
// use std::thread::sleep_for
#include <thread>
#endif // __GNUC__
#endif // PICO_BOARD

#include "meen/clock/CpuClock.h"

using namespace std::chrono;

namespace meen
{
	CpuClock::CpuClock(uint64_t speed) :
		speed_{ speed },
		timePeriod_{ 1000000000 / speed }
	{
#ifdef PICO_BOARD
		maxResolution_ = 1000ns;
#elif defined __GNUC__
		struct timespec res;
		auto err = clock_getres(CLOCK_REALTIME, &res);

		if (err < 0)
		{
			errc_ = make_error_code(errc::clock_sampling_freq);
		}
		else
		{
			maxResolution_ = nanoseconds(res.tv_nsec);
		}
#elif _WINDOWS
		auto ntdll = LoadLibrary("ntdll.dll");

		if (ntdll == nullptr)
		{
			errc_ = make_error_code(errc::clock_sampling_freq);
		}
		else
		{
			typedef long(NTAPI* pNtQueryTimerResolution)(unsigned long* MinimumResolution, unsigned long* MaximumResolution, unsigned long* CurrentResolution);
			typedef long(NTAPI* pNtSetTimerResolution)(unsigned long RequestedResolution, char SetResolution, unsigned long* ActualResolution);

			pNtQueryTimerResolution NtQueryTimerResolution = (pNtQueryTimerResolution)GetProcAddress(ntdll, "NtQueryTimerResolution");
			pNtSetTimerResolution   NtSetTimerResolution = (pNtSetTimerResolution)GetProcAddress(ntdll, "NtSetTimerResolution");

			if (NtQueryTimerResolution == nullptr || NtSetTimerResolution == nullptr)
			{
				errc_ = make_error_code(errc::clock_sampling_freq);
			}
			else
			{
				// Query for the highest accuracy timer resolution.
				unsigned long minimum, maximum, current;
				NtQueryTimerResolution(&minimum, &maximum, &current);
				// Set the timer resolution to the highest - if we do set it we MUST call it again with FALSE
				// when we exit to restore the system default.
				//NtSetTimerResolution(maximum, TRUE, &current);
				// This is the resolution that the high resolution timer should run at
				maxResolution_ = nanoseconds(maximum * 100);
			}

			// We are done
			FreeLibrary(ntdll);
		}
#else
		maxResolution_ = 40000000ns;
#endif
	}

	uint64_t CpuClock::GetSpeed() const
	{
		return speed_;
	}

	std::error_code CpuClock::SetSamplingFrequency(double samplingFrequency)
	{
		auto errc = errc_;

		if (!errc)
		{
			if (samplingFrequency > 0 && timePeriod_ > 0ns)
			{
				auto resolution = nanoseconds(static_cast<int64_t>(1000000000 / samplingFrequency));

				if (resolution < maxResolution_)
				{
					errc = make_error_code(errc::clock_sampling_freq);
				}

				totalTicks_ = resolution / timePeriod_;
			}
			else if (samplingFrequency == 0)
			{
				totalTicks_ = 0;
			}
			else
			{
				totalTicks_ = -1;
			}
		}

		return errc;
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
					if (spinTime >= 1000000ns)
					{
						auto n = nanoseconds(static_cast<int64_t>(spinTime.count() * spinPercentageToSleep_));
#ifdef PICO_BOARD
						auto now = get_absolute_time();
						sleep_us(duration_cast<microseconds>(n).count());
						spinTime -= duration_cast<nanoseconds>(microseconds(absolute_time_diff_us(now, get_absolute_time())));
#else
						auto now = steady_clock::now();
#ifdef __GNUC__
						struct timespec req
						{
							0,
#ifdef __arm__
							static_cast<int32_t>(
#endif
							n.count()
#ifdef __arm__
							)
#endif
						};

						nanosleep(&req, nullptr);
#elif defined _WINDOWS
						LARGE_INTEGER sleepPeriod;
						// Convert from nanoseconds to 100 nanosescond units, and negative for relative time.
						sleepPeriod.QuadPart = -(static_cast<int64_t>(n.count() / 100));

						// Create the timer, sleep until time has passed, and clean up - available since the 1803 version of Windows 10.
						// Sleep down to 0.5 ms intervals without raising the system level interrupt frequency, which is much friendlier.
						HANDLE timer = CreateWaitableTimerEx(nullptr, nullptr, CREATE_WAITABLE_TIMER_HIGH_RESOLUTION, TIMER_ALL_ACCESS);
						SetWaitableTimer(timer, &sleepPeriod, 0, nullptr, nullptr, 0);
						WaitForSingleObject(timer, INFINITE);
						CloseHandle(timer);
#else
						std::this_thread::sleep_for(n);
#endif
						spinTime -= duration_cast<nanoseconds>(steady_clock::now() - now);
#endif
					}

					return spinTime;
				};

				auto spinFor = [this](nanoseconds spinTime)
				{
					if(spinTime > 0ns)
					{
#ifdef PICO_BOARD
						auto now = get_absolute_time();
						busy_wait_us(duration_cast<microseconds>(spinTime).count());
						spinTime = spinTime - duration_cast<nanoseconds>(microseconds(absolute_time_diff_us(now, get_absolute_time())));
#else
						auto start = steady_clock::now();
						auto end = (start + spinTime);

						while (start < end)
						{
							start = steady_clock::now();
						}

						// record any over spin
						spinTime = duration_cast<nanoseconds>(end - start);
#endif
					}

					return spinTime;
				};

#ifdef PICO_BOARD
				auto timeTaken = duration_cast<nanoseconds>(microseconds(absolute_time_diff_us(lastTime_, get_absolute_time())));
				auto nanos = nanoseconds(tickCount_* timePeriod_) - timeTaken + error_;
#else
				auto nanos = nanoseconds(tickCount_ * timePeriod_) - duration_cast<nanoseconds>(steady_clock::now() - lastTime_) + error_;
#endif
				error_ = spinFor(sleepFor(nanos));
				tickCount_ = 0;
#ifdef PICO_BOARD
				lastTime_ = get_absolute_time();
				time_ = duration_cast<nanoseconds>(microseconds(absolute_time_diff_us(epoch_, lastTime_)));
#else
				lastTime_ = steady_clock::now();
				time_ = duration_cast<nanoseconds>(lastTime_ - epoch_);
#endif
			}
		}
		else
		{
#ifdef PICO_BOARD
			lastTime_ = get_absolute_time();
			time_ = duration_cast<nanoseconds>(microseconds(absolute_time_diff_us(epoch_, lastTime_)));
#else
			lastTime_ = steady_clock::now();
			time_ = duration_cast<nanoseconds>(lastTime_ - epoch_);
#endif
		}

		return time_;
	}

	void CpuClock::Reset()
	{
#ifdef PICO_BOARD
		epoch_ = get_absolute_time();
#else
		epoch_ = steady_clock::now();
#endif
		lastTime_ = epoch_;
	}
} // namespace meen
