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

#ifndef Z80_H
#define Z80_H

#include "8080.h"

namespace meen
{
	class Z80 final : public ICpu
	{
		private:
			/**
				z80 is backward compatible with i8080 (i8080 programs can be run on z80), so we should be able to
				declare an instance if i8080 here, call it, then use the z80 to execute unknown opcodes.
				We may need to add some config opts to the i8080 cpu if required for z80 compatibility
			*/
			Intel8080 i8080_;
		public:
			/* I8080 overrides */
			uint8_t Execute() final;
			uint8_t Interrupt(ISR isr);
			std::error_code Load(const std::string&& json, bool checkUuid) final;
#ifdef ENABLE_MEEN_SAVE
			std::expected<std::string, std::error_code> Save() const final;
#endif // ENABLE_MEEN_SAVE
			void Reset() final;
			void SetMemoryController(IController* memoryController) final;
			void SetIoController(IController* ioController) final;
			/* End I8080 overrides */

			Z80();
			~Z80();
	};
} // namespace meen

#endif // Z80_H