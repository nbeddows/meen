export module ControllerFactory;

import <memory>;
import IController;
import SystemBus;

namespace Emulator
{
	export std::unique_ptr<IMemoryController> MakeDefaultMemoryController(uint8_t addressBusSize);
	export std::unique_ptr<IController> MakeTestIoController(const std::shared_ptr<ControlBus<8>>& controlBus);
	export std::unique_ptr<IController> MakeCpmIoController(const std::shared_ptr<IController>& memoryController, const std::shared_ptr<ControlBus<8>>& controlBus);
}