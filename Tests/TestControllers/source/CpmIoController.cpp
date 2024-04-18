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

#include "Base/Base.h"
#include "TestControllers/CpmIoController.h"

namespace MachEmu
{
    CpmIoController::CpmIoController(const std::shared_ptr<IController>& memoryController)
	{
		memoryController_ = memoryController;
	}

	std::string CpmIoController::Message()
	{
		auto str = std::move(message_);
		message_.clear();
		return str;
	}

	std::array<uint8_t, 16> CpmIoController::Uuid() const
	{
		return{ 0x32, 0x8C, 0xCF, 0x78, 0x76, 0x1B, 0x48, 0xA4, 0x98, 0x2C, 0x1A, 0xAA, 0x5F, 0x14, 0x31, 0x24 };
	}

	//Not used, just return 0;
	uint8_t CpmIoController::Read(uint16_t deviceNumber)
	{
		return 0;
	}

	void CpmIoController::Write(uint16_t deviceNumber, uint8_t value)
	{
		switch (deviceNumber)
		{
			case 0:
			{
				//printf("Print Mode: %d\n", value);
				printMode_ = value;
				break;
			}
			case 1:
			{
				//printf("Addr Hi: %d\n", value);
				addrHi_ = value;
				break;
			}
			case 2:
			{
				switch (printMode_)
				{
					case 9:
					{
						uint16_t addr = (addrHi_ << 8) | value;
						uint8_t aChar = memoryController_->Read(addr);

						while (aChar != '$')
						{
							//printf("%c", aChar);
							message_.push_back(aChar);
							aChar = memoryController_->Read(++addr);
						}
						break;
					}
					case 2:
					{
						//printf ("%c", value);
						message_.push_back(value);
						break;
					}
					default:
					{
						//assert(0);
					}
				}
				break;
			}
			default:
			{
				BaseIoController::Write(deviceNumber, value);
			}
		}
	}

	//No interrupts
	ISR CpmIoController::ServiceInterrupts(uint64_t currTime, uint64_t cycles)
	{
		return BaseIoController::ServiceInterrupts(currTime, cycles);
	}
} // namespace MachEmu