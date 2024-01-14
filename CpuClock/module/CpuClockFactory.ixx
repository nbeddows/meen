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

export module CpuClockFactory;

import <chrono>;
import <memory>;
import ICpuClock;
import SystemBus;

namespace MachEmu
{
	/** Factory function to make a cpu clock
	
		The CPU clock contols the timing for the target CPU so that all instructions executed
		execute at the correct rate.

		@param	correlateFrequency		The frequency in milliseconds at which the clock will sync
										The target cpu to the correct rate.

										A frequency of 0 will sync the cpu at every call to Tick() (this will spin the
										cpu 100% of the time to maintain sync (most accurate, high cpu usage)).
										A frequency of -1 will not perform any synchronisation (run as fast as possible).

										The lower the frequency the higher the resolution of the clock (0 being the highest, ie;
										sync the cpu after every instruction).
										Low correlation frequencies (above 0) can yield inaccurate clock speeds when a high
										resolution clock is not available (anything below 30-50 milliseconds). When a high resolution
										clock is available the best resolution attainable is at most 1 millisecond.

										@see ICpuClock::Tick

		@param	speed					The desired clock speed in ticks per second, for the i8080 cpu this will be 2000000 (2Mhz)

		@return	unique_ptr				A unique_ptr to the CpuClock interface.
	*/
	export std::unique_ptr<ICpuClock> MakeCpuClock(std::chrono::milliseconds correlateFreq, uint64_t speed);
} // namespace MachEmu