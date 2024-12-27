//#include <format> 
#include <pybind11/pybind11.h>

#include "meen/machine_py/MachineHolder.h"

namespace meen
{
	MachineHolder::MachineHolder(Cpu cpu)
	{
		switch(cpu)
		{
			case Cpu::i8080:
			{
				machine_ = meen::Make8080Machine();
			}
			default:
				break;
		}
	}

	errc MachineHolder::OnLoad(std::function<std::string()>&& onLoad)
	{
		auto err = machine_->OnLoad([this, ol = std::move(onLoad)]
		{
			json_ = ol();
			return json_.c_str();
		});

		return static_cast<errc>(err.value());
	}

	errc MachineHolder::OnSave(std::function<void(std::string&&)>&& onSave)
	{
		auto err = machine_->OnSave([os = std::move(onSave)](const char* json)
		{
			os(json);
		});

		return static_cast<errc>(err.value());
	}

	errc MachineHolder::Run(uint16_t offset)
	{
		auto err = machine_->Run(offset);
		return static_cast<errc>(err.value());
	}

	errc MachineHolder::SetIoController(meen::IController* controller)
	{
		// custom deleter, don't delete this pointer from c++, python owns it
		auto err = machine_->SetIoController(std::shared_ptr<meen::IController>(controller, [](meen::IController*) {}));
		return static_cast<errc>(err.value());
	}

	errc MachineHolder::SetMemoryController(meen::IController* controller)
	{
		auto err = machine_->SetMemoryController(std::shared_ptr<meen::IController>(controller, [](meen::IController*) {}));
		return static_cast<errc>(err.value());
	}

	errc MachineHolder::SetOptions(const char* options)
	{
		auto err = machine_->SetOptions(options);
		return static_cast<errc>(err.value());
	}

	uint64_t MachineHolder::WaitForCompletion()
	{
		// WaitForCompletion is a long running function that does not interract with Python.
		// Release the Python Global Interpreter Lock so the calling script doesn't stall.
		pybind11::gil_scoped_release nogil{};
		return machine_->WaitForCompletion().value_or(0);
	}

	MachineHolder MachineHolder::Make8080Machine()
	{
		return MachineHolder(Cpu::i8080);
	}
} // namespace meen
