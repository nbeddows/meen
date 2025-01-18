#include <pybind11/functional.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "meen/Error.h"
#include "meen/MachineFactory.h"
#include "meen/machine_py/ControllerPy.h"

namespace py = pybind11;

//cppcheck-suppress unusedFunction
PYBIND11_MODULE(meenPy, meen)
{        
    meen.attr("__version__") = meen::Version();

    py::enum_<meen::errc>(meen, "ErrorCode")
        .value("NoError", meen::errc::no_error)
        .value("Async", meen::errc::async)
        .value("Busy", meen::errc::busy)
        .value("ClockResolution", meen::errc::clock_resolution)
        .value("Cpu", meen::errc::cpu)
        .value("IncompatibleRam", meen::errc::incompatible_ram)
        .value("IncompatibleRom", meen::errc::incompatible_rom)
        .value("IncompatibleUuid", meen::errc::incompatible_uuid)
        .value("InvalidArgument", meen::errc::invalid_argument)
        .value("IoController", meen::errc::io_controller)
        .value("JsonConfig", meen::errc::json_config)
        .value("JsonParse", meen::errc::json_parse)
        .value("MemoryController", meen::errc::memory_controller)
        .value("NoZlib", meen::errc::no_zlib)
        .value("NotImplemented", meen::errc::not_implemented)
        .value("UnknownOption", meen::errc::unknown_option);

    py::enum_<meen::ISR>(meen, "ISR")
        .value("Zero", meen::ISR::Zero)
        .value("One", meen::ISR::One)
        .value("Two", meen::ISR::Two)
        .value("Three", meen::ISR::Three)
        .value("Four", meen::ISR::Four)
        .value("Five", meen::ISR::Five)
        .value("Six", meen::ISR::Six)
        .value("Seven", meen::ISR::Seven)
        .value("Load", meen::ISR::Load)
        .value("Save", meen::ISR::Save)
        .value("Quit", meen::ISR::Quit)
        .value("NoInterrupt", meen::ISR::NoInterrupt);

    meen.def("Make8080Machine", &meen::Make8080Machine);
    
    py::class_<meen::IMachine>(meen, "IMachine")
        .def("OnLoad", [](meen::IMachine& machine, std::function<std::string()>&& onLoad)
        {
            return static_cast<meen::errc>(machine.OnLoad([ol = std::move(onLoad)](char* json, int* jsonLen)
            {
                auto str = ol();

                if(jsonLen == nullptr)
                {
                    return meen::errc::invalid_argument;
                }

                if (json == nullptr)
                {
                    *jsonLen = str.length();
                }
                else
                {
                    if (str.length() > *jsonLen)
                    {
                        return meen::errc::invalid_argument;
                    }

                    strncpy(json, str.c_str(), *jsonLen);
                }
                
                return meen::errc::no_error;
            }).value());            
        })
        .def("OnSave", [](meen::IMachine& machine, std::function<meen::errc(std::string&& json)>&& onSave)
        {
            return static_cast<meen::errc>(machine.OnSave(std::move(onSave)).value());
        })
        .def("Run", [](meen::IMachine& machine, uint16_t offset)
        {
            return static_cast<meen::errc>(machine.Run(offset).value());
        })
        .def("AttachIoController", [](meen::IMachine& machine, meen::IController* controller)
        {
            return static_cast<meen::errc>(machine.AttachIoController(meen::IControllerPtr(controller, meen::ControllerDeleter(false))).value());            
        })
        .def("AttachMemoryController", [](meen::IMachine& machine, meen::IController* controller)
        {
            return static_cast<meen::errc>(machine.AttachMemoryController(meen::IControllerPtr(controller, meen::ControllerDeleter(false))).value());
        })
        .def("SetOptions", [](meen::IMachine& machine, const char* options)
        {
            return static_cast<meen::errc>(machine.SetOptions(options).value());
        })
        .def("WaitForCompletion", [](meen::IMachine& machine)
        {
            pybind11::gil_scoped_release nogil{};
		    return machine.WaitForCompletion().value_or(0);
        });

    py::class_<meen::IController, meen::ControllerPy>(meen, "Controller")
        .def(py::init<>())
        .def("Read", &meen::IController::Read)
        .def("Write", &meen::IController::Write)
        .def("ServiceInterrupts", &meen::IController::ServiceInterrupts)
        .def("Uuid", &meen::IController::Uuid);
}
