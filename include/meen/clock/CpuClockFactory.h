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

#ifndef CPUCLOCK_FACTORY_H
#define CPUCLOCK_FACTORY_H

#include <memory>

#include "meen/clock/ICpuClock.h"

namespace meen
{
	/** Factory function to make a cpu clock
	
		The CPU clock contols the timing for the target CPU so that all instructions executed
		execute at the correct rate.

		@param	speed					The desired clock speed in ticks per second, for the i8080 cpu this will be 2000000 (2Mhz)

		@return	unique_ptr				A unique_ptr to the CpuClock interface.
	*/
	std::unique_ptr<ICpuClock> MakeCpuClock(uint64_t speed);
} // namespace meen

#endif // CPUCLOCK_FACTORY_H