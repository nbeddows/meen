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

#include <fstream>

#include "meen/Base.h"
#include "test_controllers/MemoryController.h"

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

	int MemoryController::Load(const char* romFile, uint16_t offset)
	{
		std::ifstream fin(romFile, std::ios::binary | std::ios::ate);

		if (!fin)
		{
			return -1;
		}

		if (static_cast<size_t>(fin.tellg()) > memorySize_)
		{
			return -1;
		}

		uint16_t size = static_cast<uint16_t>(fin.tellg());

		if (size > memorySize_ - offset)
		{
			return -1;
		}

		fin.seekg(0, std::ios::beg);

		if (!(fin.read(reinterpret_cast<char*>(&memory_[offset]), size)))
		{
			return -1;
		}

		return 0;
	}

	std::array<uint8_t, 16> MemoryController::Uuid() const
	{
		return{ 0xCD, 0x18, 0xD8, 0x67, 0xDD, 0xBF, 0x4D, 0xAA, 0xAD, 0x5A, 0xBA, 0x1C, 0xEB, 0xAE, 0xB0, 0x31 };
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

	ISR MemoryController::ServiceInterrupts([[maybe_unused]] uint64_t currTime, [[maybe_unused]] uint64_t cycles)
	{
		// this controller never issues any interrupts
		return ISR::NoInterrupt;
	}
} // namespace meen