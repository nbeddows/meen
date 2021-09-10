export module I8080;

import <cstdint>;
import <memory>;
import <string_view>;
import <vector>;
import SystemBus;

namespace Emulator
{
	export struct I8080
	{
		//Executes the next instruction
		virtual bool Execute() = 0;

		virtual void Reset() = 0;

		//virtual void ProgramCounter() = 0;

		//virtual void Save() const = 0;

		virtual ~I8080() = default;
	};
	
	export std::unique_ptr<I8080> Make8080(const SystemBus<uint16_t, uint8_t, 8>& systemBus);
}