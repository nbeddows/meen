export module Machine;

import I8080;
import IController;
import ICpuClock;
import IMachine;
import SystemBus;

namespace Emulator
{
	/** Machine

	*/
	export struct Machine final : public IMachine
	{
	private:
		std::unique_ptr<ICpuClock> clock_;
		std::unique_ptr<I8080> cpu_;
		std::shared_ptr<IController> memoryController_;
		std::shared_ptr<IController> ioController_;
		SystemBus<uint16_t, uint8_t, 8> systemBus_;
	public:
		Machine();

		/** Run
		
			@see IMachine::Run
		*/
		void Run(uint16_t pc) override final;

		/** SetMemoryController

			@see IMachine::SetMemoryController
		*/
		void SetMemoryController(const std::shared_ptr<IController>& controller) override final;

		/** SetIoController

			@see IMachine::SetIoController
		*/
		void SetIoController(const std::shared_ptr<IController>& controller) override final;
	};
}