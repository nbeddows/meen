//#include <format> 
#include <pybind11/pybind11.h>

#include "meen/machine_py/MachineHolder.h"

namespace MachEmu
{
	MachineHolder::MachineHolder()
	{
		machine_ = MachEmu::MakeMachine();
	}

	MachineHolder::MachineHolder(const char* json)
	{
		machine_ = MachEmu::MakeMachine(json);
	}

	errc MachineHolder::SetClockResolution(int64_t clockResolution)
	{
		auto count = snprintf(nullptr, 0, "{\"clockResolution\":%" PRIi64 "}", clockResolution);
		auto str = std::string(count + 1, '\0');
		snprintf(str.data(), count + 1, "{\"clockResolution\":%" PRIi64 "}", clockResolution);
		auto err = machine_->SetOptions(str.c_str());
		return static_cast<errc>(err.value());
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

	std::string MachineHolder::Save() const
	{
		return machine_->Save();
	}

	uint64_t MachineHolder::Run(uint16_t offset)
	{
		return machine_->Run(offset);
	}

	errc MachineHolder::SetIoController(MachEmu::IController* controller)
	{
		// custom deleter, don't delete this pointer from c++, python owns it
		auto err = machine_->SetIoController(std::shared_ptr<MachEmu::IController>(controller, [](MachEmu::IController*) {}));
		return static_cast<errc>(err.value());
	}

	errc MachineHolder::SetMemoryController(MachEmu::IController* controller)
	{
		auto err = machine_->SetMemoryController(std::shared_ptr<MachEmu::IController>(controller, [](MachEmu::IController*) {}));
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
		return machine_->WaitForCompletion();
	}
} // namespace MachEmu
