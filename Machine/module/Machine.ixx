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
		//The only reason this is a shared_ptr is because we need to return it so we can run the cpu tests.
		std::shared_ptr<I8080> cpu_;
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
		void SetMemoryController(std::shared_ptr<IController> controller) override final;

		/** SetIoController

			@see IMachine::SetIoController
		*/
		void SetIoController(std::shared_ptr<IController> controller) override final;

		/** ControlBus
		
			@see IMachine::ControlBus
		*/
		const std::shared_ptr<Emulator::ControlBus<8>>& ControlBus() const override final;

		/** Cpu
		
			Returns the internal Cpu.

			@return I8080	The cpu used by this machine.

			@discussion		This function is only used during the cpu tests.
		*/
		const std::shared_ptr<I8080>& Cpu() const;
	};
}