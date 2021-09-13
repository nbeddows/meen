export module Machine;

import I8080;
import IController;
import CpuClock;
import IMachine;
import SystemBus;

namespace Emulator
{
	/** Machine

	*/
	export struct Machine final : public IMachine
	{
	private:
		std::unique_ptr<CpuClock> clock_;
		std::unique_ptr<I8080> cpu_;
		std::shared_ptr<IMemoryController> memoryController_;
		std::shared_ptr<IController> ioController_;
		SystemBus<uint16_t, uint8_t, 8> systemBus_;
	public:
		/** Load
		
			@see IMachine::Load
		*/
		void Load(std::filesystem::path rom, uint32_t memoryOffset) override final;

		/** Run
		
			@see IMachine::Run
		*/
		void Run(uint16_t pc) override final;

		/** SetMemoryController

			@see IMachine::SetMemoryController
		*/
		void SetMemoryController(std::shared_ptr<IController> controller) override final;

		/** SetIoController

			@see IMachine::SetIoController
		*/
		void SetIoController(std::shared_ptr<IController> controller) override final;
	};
}