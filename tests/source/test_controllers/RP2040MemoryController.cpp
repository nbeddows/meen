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

#include <cstdio>

#include <cstring>
#include <string_view>

#include "meen/Base.h"
#include "test_controllers/MemoryController.h"

// Extern variables from TestPrograms.S
extern uint8_t bdosMsgStart;
extern uint8_t bdosMsgEnd;
extern uint8_t exitTestStart;
extern uint8_t exitTestEnd;
extern uint8_t nopStartStart;
extern uint8_t nopStartEnd;
extern uint8_t nopEndStart;
extern uint8_t nopEndEnd;
extern uint8_t cpuTestStart;
extern uint8_t cpuTestEnd;
extern uint8_t pre8080Start;
extern uint8_t pre8080End;
extern uint8_t exm8080Start;
extern uint8_t exm8080End;
extern uint8_t tst8080Start;
extern uint8_t tst8080End;

namespace meen
{
	MemoryController::MemoryController()
	{
		memory_.resize(memorySize_);
	}

	size_t MemoryController::Size() const
	{
		return memorySize_;
	}

	std::error_code MemoryController::Load(const char* romFile, uint16_t offset)
	{
		auto sv = std::string_view(romFile, strlen(romFile));

		auto copyFromFlashToRam = [this, offset](uint8_t* src, uint16_t srcSize)
		{
			if (srcSize > memory_.size())
			{
				return std::make_error_code(std::errc::file_too_large);
			}
		
			if(srcSize > memory_.size() - offset)
			{
				return std::make_error_code(std::errc::no_buffer_space);
			}

			memcpy(memory_.data() + offset, src, srcSize);
			return std::error_code{};
		};

		if(sv.ends_with("bdosMsg.bin") == true)
		{
			return copyFromFlashToRam(&bdosMsgStart, &bdosMsgEnd - &bdosMsgStart);
		}
		else if (sv.ends_with("exitTest.bin") == true)
		{
			return copyFromFlashToRam(&exitTestStart, &exitTestEnd - &exitTestStart);
		}
		else if (sv.ends_with("nopStart.bin") == true)
		{
			return copyFromFlashToRam(&nopStartStart, &nopStartEnd - &nopStartStart);
		}
		else if (sv.ends_with("nopEnd.bin") == true)
		{
			return copyFromFlashToRam(&nopEndStart, &nopEndEnd - &nopEndStart);
		}
		else if (sv.ends_with("8080PRE.COM") == true)
		{
			return copyFromFlashToRam(&pre8080Start, &pre8080End - &pre8080Start);
		}
		else if (sv.ends_with("8080EXM.COM") == true)
		{
			return copyFromFlashToRam(&exm8080Start, &exm8080End - &exm8080Start);
		}
		else if (sv.ends_with("CPUTEST.COM") == true)
		{
			return copyFromFlashToRam(&cpuTestStart, &cpuTestEnd - &cpuTestStart);
		}
		else if (sv.ends_with("TST8080.COM") == true)
		{
			return copyFromFlashToRam(&tst8080Start, &tst8080End - &tst8080Start);
		}
		else
		{
			return std::error_code{};
		}
	}

	std::array<uint8_t, 16> MemoryController::Uuid() const
	{
		return{ 0x6A, 0x23, 0xC8, 0x6D, 0x56, 0xEE, 0x45, 0x79, 0xAD, 0xB3, 0x92, 0x63, 0xED, 0x74, 0x58, 0x33 };
	}

	uint8_t MemoryController::Read(uint16_t addr)
	{
		return memory_[addr];
	}

	void MemoryController::Write(uint16_t addr, uint8_t data)
	{
		memory_[addr] = data;
	}

	void MemoryController::Clear()
	{
		memory_.assign(memory_.size(), 0);
	}

	ISR MemoryController::ServiceInterrupts([[maybe_unused]] uint64_t currTime, [[maybe_unused]] uint64_t cycles, [[maybe_unused]] IController* ioController)
	{
		// this controller never issues any interrupts
		return ISR::NoInterrupt;
	}
} // namespace meen
