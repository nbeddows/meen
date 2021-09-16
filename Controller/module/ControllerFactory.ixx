export module ControllerFactory;

import <memory>;
import IController;

namespace Emulator
{
	export std::unique_ptr<IMemoryController> MakeDefaultMemoryController(uint8_t addressBusSize);
	export std::unique_ptr<IController> MakeTestIoController();
	export std::unique_ptr<IController> MakeCpmIoController(const std::shared_ptr<IController>& memoryController);
}