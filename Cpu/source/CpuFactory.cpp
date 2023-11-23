//module;
//#include <memory>

module I8080;

import <memory>;
import _8080;

namespace Emulator
{
	std::unique_ptr<I8080> Make8080(const SystemBus<uint16_t, uint8_t, 8>& systemBus, std::function<void(const SystemBus<uint16_t, uint8_t, 8>&&)> process)
	{
		return std::make_unique<Intel8080>(systemBus, process);
	}
}