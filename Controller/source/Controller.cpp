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

module;
#include <chrono>
#include <cmath>
#include <fstream>

module Controller;

using namespace std::chrono;

namespace Emulator
{
	DefaultMemoryController::DefaultMemoryController(uint8_t addrSize)
	{
		memorySize_ = static_cast<size_t>(std::pow(2, addrSize));
		memory_ = std::make_unique<uint8_t[]>(memorySize_);
	}

	size_t DefaultMemoryController::Size() const
	{
		return memorySize_;
	}

	void DefaultMemoryController::Load(std::filesystem::path romFile, uint16_t offset)
	{
		std::ifstream fin(romFile, std::ios::binary | std::ios::ate);

		if (!fin)
		{
			throw std::runtime_error("The program file failed to open");
		}

		if (static_cast<size_t>(fin.tellg()) > memorySize_)
		{
			throw std::length_error("The length of the program is too big");
		}

		uint16_t size = static_cast<uint16_t>(fin.tellg());

		if (size > memorySize_ - offset)
		{
			throw std::length_error("The length of the program is too big to fit at the specified offset");
		}

		fin.seekg(0, std::ios::beg);

		if (!(fin.read(reinterpret_cast<char*>(&memory_[offset]), size)))
		{
			throw std::invalid_argument("The program specified failed to load");
		}
	}

	uint8_t DefaultMemoryController::Read(uint16_t addr)
	{
		return memory_[addr];
	}

	void DefaultMemoryController::Write(uint16_t addr, uint8_t data)
	{
		memory_[addr] = data;
	}

	ISR DefaultMemoryController::ServiceInterrupts([[maybe_unused]] nanoseconds currTime, [[maybe_unused]] uint64_t cycles)
	{
		//this controller never issues any interrupts
		return ISR::NoInterrupt;
	}

	void MockIoController::Write(uint16_t port, [[maybe_unused]] uint8_t value)
	{
		powerOff_ = port == 0xFF;
	}

	ISR MockIoController::ServiceInterrupts([[maybe_unused]] nanoseconds currTime, [[maybe_unused]] uint64_t cycles)
	{
		auto isr = ISR::NoInterrupt;

		if (powerOff_ == true)
		{
			isr = ISR::Quit;
			powerOff_ = false;
		}
		
		return isr;
	}

	uint8_t TestIoController::Read(uint16_t deviceNumber)
	{
		uint8_t deviceData = 0x00;

		switch (deviceNumber)
		{
			case 0x00:
			{
				//return the data of device 0.
				deviceData = deviceData_;
				break;
			}
			default:
			{
				//we don't support other devices, return 0.
				break;
			}
		}

		return deviceData;
	}

	void TestIoController::Write(uint16_t deviceNumber, uint8_t value)
	{
		switch (deviceNumber)
		{
			case 0:
			{
				//Update device 0 with the contents of the accumulator.
				deviceData_ = value;
				break;
			}
			default:
			{
				MockIoController::Write (deviceNumber, value);
				break;
			}
		}
	}

	ISR TestIoController::ServiceInterrupts(nanoseconds currTime, [[maybe_unused]] uint64_t cycles)
	{
		auto isr = MockIoController::ServiceInterrupts(currTime, cycles);

		if (isr == ISR::NoInterrupt)
		{
			//Fire interrupt rst 1 every second, the cpu will only acknowledge
			//the interrupt if the test programs have interrupts enabled,
			//otherwise it will be ignored.
			auto t = currTime - lastTime_;

			if (t >= nanoseconds::zero())
			{
				if (t > nanoseconds(1000000000))
				{
					lastTime_ = currTime;
					isr = ISR::One;
				}
			}
			else
			{
				//If the cpu clock gets restarted for example,
				//lastTime will be stale, so we reset it.
				lastTime_ = currTime;
			}
		}

		return isr;
	}

	CpmIoController::CpmIoController(const std::shared_ptr<IController>& memoryController)
	{
		memoryController_ = memoryController;
	}

	std::string CpmIoController::Message()
	{
		return std::move(message_);
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
				MockIoController::Write(deviceNumber, value);
			}
		}
	}

	//No interrupts
	ISR CpmIoController::ServiceInterrupts(nanoseconds currTime, uint64_t cycles)
	{
		return MockIoController::ServiceInterrupts(currTime, cycles);
	}
}