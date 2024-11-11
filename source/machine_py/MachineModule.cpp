#include <pybind11/functional.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#define ENABLE_MACHINE_HOLDER

#ifdef ENABLE_MACHINE_HOLDER
    #include "meen/machine_py/MachineHolder.h"
#else
    #include "meen/MachineFactory.h"
#endif

#include "meen/machine_py/ControllerPy.h"

namespace py = pybind11;

//cppcheck-suppress unusedFunction
PYBIND11_MODULE(mach_emuPy, meen)
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

#ifdef ENABLE_MACHINE_HOLDER
    py::class_<meen::MachineHolder>(meen, "MakeMachine")
        .def(py::init<>())
        .def(py::init<const char*>())
        .def("OnLoad", &meen::MachineHolder::OnLoad)
        .def("OnSave", &meen::MachineHolder::OnSave)
        .def("Run", &meen::MachineHolder::Run)
        .def("Save", &meen::MachineHolder::Save)
        .def("SetClockResolution", &meen::MachineHolder::SetClockResolution)
        .def("SetIoController", &meen::MachineHolder::SetIoController)
        .def("SetMemoryController", &meen::MachineHolder::SetMemoryController)
        .def("SetOptions", &meen::MachineHolder::SetOptions)
        .def("WaitForCompletion", &meen::MachineHolder::WaitForCompletion);
#else
    meen.def("Make8080Machine", &meen::Make8080Machine);

    py::class_<meen::IMachine, std::shared_ptr<meen::IMachine>>(meen, "IMachine")
        .def("Run", &meen::IMachine::Run)
        .def("SetClockResolution", &meen::IMachine::SetClockResolution)
        .def("SetIoController", &meen::IMachine::SetIoController);
        .def("SetMemoryController", &meen::IMachine::SetMemoryController);
        .def("SetOptions", &meen::IMachine::SetOptions)
        .def("WaitForCompletion", &meen::IMachine::WaitForCompletion);
#endif

    py::class_<meen::IController, meen::ControllerPy>(meen, "Controller")
        .def(py::init<>())
        .def("Read", &meen::IController::Read)
        .def("Write", &meen::IController::Write)
        .def("ServiceInterrupts", &meen::IController::ServiceInterrupts)
        .def("Uuid", &meen::IController::Uuid);
}
