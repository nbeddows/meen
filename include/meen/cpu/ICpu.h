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

#ifndef ICPU_H
#define ICPU_H

#include <cstdint>
#include <memory>
#include <string>
#include <system_error>

#include "meen/Base.h"
#include "meen/IController.h"

namespace MachEmu
{
	struct ICpu
	{
		virtual std::error_code SetMemoryController(const std::shared_ptr<IController>& memoryController) = 0;
		
		virtual std::error_code SetIoController(const std::shared_ptr<IController>& ioController) = 0;

		//Executes the next instruction
		virtual uint8_t Execute() = 0;

		virtual uint8_t Interrupt(ISR isr) = 0;

		virtual void Reset(uint16_t pc) = 0;

		virtual std::unique_ptr<uint8_t[]> GetState(int* size) const = 0;

#ifdef ENABLE_MEEN_SAVE
		virtual std::error_code Load(const std::string&& json) = 0;

		virtual std::string Save() const = 0;
#endif // ENABLE_MEEN_SAVE
		virtual ~ICpu() = default;
	};
} // namespace MachEmu

#endif // ICPU_H