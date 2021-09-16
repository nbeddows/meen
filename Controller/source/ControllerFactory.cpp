module ControllerFactory;

import <memory>;
import Controller;

namespace Emulator
{
	std::unique_ptr<IMemoryController> MakeDefaultMemoryController(uint8_t addressBusSize)
	{
		return std::make_unique<DefaultMemoryController>(addressBusSize);
	}

	std::unique_ptr<IController> MakeTestIoController()
	{
		return std::make_unique<TestIoController>();
	}

	std::unique_ptr<IController> MakeCpmIoController(const std::shared_ptr<IController>& memoryController)
	{
		return std::make_unique<CpmIoController>(memoryController);
	}
}
