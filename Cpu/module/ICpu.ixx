export module I8080;

import <cstdint>;
import <functional>;
import <memory>;
import <string_view>;
import <vector>;
import SystemBus;

namespace Emulator
{
	export struct I8080
	{
		//Executes the next instruction
		//virtual bool Execute() = 0;
		virtual uint8_t Execute() = 0;

		virtual void Reset(uint16_t pc) = 0;

		//virtual void Save() const = 0;

		virtual ~I8080() = default;
	};

	export std::unique_ptr<I8080> Make8080(const SystemBus<uint16_t, uint8_t, 8>& systemBus, std::function<void(const SystemBus<uint16_t, uint8_t, 8>&&)> process);
}