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

#include "gtest/gtest.h"

import ICpuClock;
import <chrono>;

using namespace std::chrono;

namespace Emulator::CpuClock::Tests
{
	class CpuClockTest : public testing::Test, public testing::WithParamInterface<std::shared_ptr<ICpuClock>>
	{
	protected:
		void RunClockForWithTicks(uint32_t secs, uint16_t ticks);
		void RunClockFor(uint32_t secs);
	public:
		void SetUp();
	};

	void CpuClockTest::SetUp()
	{
		GetParam()->Reset();
	}

	void CpuClockTest::RunClockForWithTicks(uint32_t secs, uint16_t ticks)
	{
		auto clock = GetParam();
		auto targetTime = nanoseconds::zero();
		auto hostTime = nanoseconds::zero();
		int over2microsCount = 0;

		//stop after what the host believes is 'secs' seconds
		while (duration_cast<seconds>(targetTime) < seconds(secs))
		{
			hostTime = clock->Tick(ticks);
			targetTime = clock->Time();

			//record the amount of times the host was too slow (anything over 2 micros)
			if (hostTime - targetTime > nanoseconds(2000))
			{
				over2microsCount++;
			}
		}

		//+- 5 micros of tolerance
		//this can fail on the odd occasion if the host happens to stall
		//on the last iteration for reasons that are beyond our control,
		//for example, we overslept on the final Tick call 
		EXPECT_LT (abs((hostTime - targetTime).count()), 5000);

		double error = over2microsCount / ((secs * static_cast<uint64_t>(1000000000)) / (static_cast<double>(ticks) * clock->TimePeriod().count()));

		//allow the host to be slower by more than 2 micros than the target NO more than
		//0.1% of the time
		EXPECT_LT(error, 0.1);
	}

	void CpuClockTest::RunClockFor(uint32_t secs)
	{
		auto clock = GetParam();
		auto runTime = nanoseconds::zero();
		auto targetTime = duration_cast<nanoseconds>(seconds(secs));

		while (runTime < targetTime)
		{
			runTime += clock->Tick();
		}

		EXPECT_GE(runTime, targetTime);
		EXPECT_LT(runTime - targetTime, nanoseconds(5000));
	}

	//cppcheck-suppress syntaxError
	TEST_P(CpuClockTest, RunClockFor2Secs)
	{
		RunClockFor(2);
	}

	TEST_P(CpuClockTest, RunClockFor4Secs)
	{
		RunClockFor(4);
	}

	TEST_P(CpuClockTest, RunClockFor2SecsWith4TicksPerCall)
	{
		RunClockForWithTicks(2, 4);
	}

	TEST_P(CpuClockTest, RunClockFor2SecsWith40kTicksPerCall)
	{
		RunClockForWithTicks(2, 40000);
	}

	//A time period may vary from 480 nanosecs to 2000 nanosecs for i8080
	//INSTANTIATE_TEST_CASE_P(CpuClockTests,
	//						CpuClockTest,
	//						testing::Values(
	//							std::shared_ptr<ICpuClock>(MakeCpuClock(nanoseconds(2000), nanoseconds(0))),
	//							std::shared_ptr<ICpuClock>(MakeCpuClock(nanoseconds(2000), nanoseconds(80000000))),
	//							std::shared_ptr<ICpuClock>(MakeCpuClock(nanoseconds(1000), nanoseconds(0))),
	//							std::shared_ptr<ICpuClock>(MakeCpuClock(nanoseconds(1000), nanoseconds(80000000)))
	//						));
};