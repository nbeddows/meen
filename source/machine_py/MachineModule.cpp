#include <pybind11/functional.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "meen/machine/Machine.h"
#include "meen/machine_py/ControllerPy.h"
#include "meen/machine_py/MachineHolder.h"

namespace py = pybind11;

//cppcheck-suppress unusedFunction
PYBIND11_MODULE(meenPy, meen)
{        
    meen.attr("__version__") = meen::Version();

    py::enum_<meen::errc>(meen, "ErrorCode")
        .value("NoError", meen::errc::no_error)
        .value("Busy", meen::errc::busy)
        .value("ClockResolution", meen::errc::clock_resolution)
        .value("InvalidArgument", meen::errc::invalid_argument)
        .value("JsonConfig", meen::errc::json_config)
        .value("JsonParse", meen::errc::json_parse)
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

    meen.def("Make8080Machine", &meen::MachineHolder::Make8080Machine);
    
    py::class_<meen::MachineHolder>(meen, "Machine")
        .def(py::init<meen::Cpu>())
        .def("OnLoad", &meen::MachineHolder::OnLoad)
        .def("OnSave", &meen::MachineHolder::OnSave)
        .def("Run", &meen::MachineHolder::Run)
        .def("SetIoController", &meen::MachineHolder::SetIoController)
        .def("SetMemoryController", &meen::MachineHolder::SetMemoryController)
        .def("SetOptions", &meen::MachineHolder::SetOptions)
        .def("WaitForCompletion", &meen::MachineHolder::WaitForCompletion);

    py::class_<meen::IController, meen::ControllerPy>(meen, "Controller")
        .def(py::init<>())
        .def("Read", &meen::IController::Read)
        .def("Write", &meen::IController::Write)
        .def("ServiceInterrupts", &meen::IController::ServiceInterrupts)
        .def("Uuid", &meen::IController::Uuid);
}
