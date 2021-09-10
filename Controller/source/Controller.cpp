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

	ISR DefaultMemoryController::ServiceInterrupts([[maybe_unused]] nanoseconds currTime)
	{
		//this controller never issues any interrupts
		return ISR::NoInterrupt;
	}

	MockIoController::MockIoController(const std::shared_ptr<ControlBus<8>>& controlBus)
	{
		controlBus_ = controlBus;
	}

	void MockIoController::Write(uint16_t port, [[maybe_unused]] uint8_t value)
	{
		powerOff_ = port == 0xFF;
	}

	ISR MockIoController::ServiceInterrupts(nanoseconds currTime)
	{
		if (powerOff_ == true)
		{
			controlBus_->Send(Signal::PowerOff);
			powerOff_ = false;
		}
		
		return ISR::NoInterrupt;
	}

	TestIoController::TestIoController(const std::shared_ptr<ControlBus<8>>& controlBus) : MockIoController(controlBus)
	{
	
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

	ISR TestIoController::ServiceInterrupts(nanoseconds currTime)
	{
		auto isr = ISR::NoInterrupt;

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
		
		MockIoController::ServiceInterrupts(currTime);
		return isr;
	}

	CpmIoController::CpmIoController(const std::shared_ptr<IController>& memoryController, const std::shared_ptr<ControlBus<8>>& controlBus) : MockIoController(controlBus)
	{
		memoryController_ = memoryController;
	}

	const std::string& CpmIoController::Message() const
	{
		return message_;
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
				printMode_ = value;
				break;
			}
			case 1:
			{
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
							if (isprint(aChar))
							{
								message_.push_back(aChar);
							}

							aChar = memoryController_->Read(++addr);
						}
						break;
					}
					case 2:
					{
						if (isprint(value))
						{
							message_.push_back(value);
						}
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
	ISR CpmIoController::ServiceInterrupts([[maybe_unused]] nanoseconds currTime)
	{
		MockIoController::ServiceInterrupts(currTime);

		return ISR::NoInterrupt;
	}
}