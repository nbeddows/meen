/*
Copyright (c) 2021-2026 Nicolas Beddows <nicolas.beddows@gmail.com>

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

#include <assert.h>

#include "meen/cpu/Z80.h"

namespace meen
{
	Z80::Z80()
	{

	}

	Z80::~Z80()
	{

	}

	/* I8080 overrides */
	int8_t Z80::Execute()
	{
		auto timePeriods = i8080_.Execute();

		if (timePeriods < 0)
		{
			uint8_t opcode = 0;
			/*
				Read the next opcode
			*/
			switch (opcode)
			{
				case 0xCB:
					break;
				case 0xDD:
					break;
				case 0xED:
					break;
				case 0xFD:
					break;
				default:
					assert(0);
					break;
			}
		}
		
		return timePeriods;
	}

	int8_t Z80::Interrupt(ISR isr)
	{
		auto timePeriods = i8080_.Interrupt(isr);

		if (timePeriods < 0)
		{
			
		}

		return timePeriods;
	}

	std::error_code Z80::Load(const std::string&& json, bool checkUuid)
	{
		return i8080_.Load(std::move(json), checkUuid);
	}
#ifdef ENABLE_MEEN_SAVE
	std::expected<std::string, std::error_code> Z80::Save() const
	{
		return i8080_.Save();
	}
#endif // ENABLE_MEEN_SAVE
	void Z80::Reset()
	{
		i8080_.Reset();
	}

	void Z80::SetMemoryController(IController* memoryController)
	{
		i8080_.SetMemoryController(memoryController);
	}

	void Z80::SetIoController(IController* ioController)
	{
		i8080_.SetIoController(ioController);
	}
	/* End I8080 overrides */
}
