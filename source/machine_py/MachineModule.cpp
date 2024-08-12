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
PYBIND11_MODULE(mach_emuPy, MachEmu)
{        
    MachEmu.attr("__version__") = MachEmu::Version();

    py::enum_<MachEmu::errc>(MachEmu, "ErrorCode")
        .value("NoError", MachEmu::errc::no_error)
        .value("ClockResolution", MachEmu::errc::clock_resolution)
        .value("NotImplemented", MachEmu::errc::not_implemented)
        .value("UnknownOption", MachEmu::errc::unknown_option);
    
    py::enum_<MachEmu::ISR>(MachEmu, "ISR")
        .value("Zero", MachEmu::ISR::Zero)
        .value("One", MachEmu::ISR::One)
        .value("Two", MachEmu::ISR::Two)
        .value("Three", MachEmu::ISR::Three)
        .value("Four", MachEmu::ISR::Four)
        .value("Five", MachEmu::ISR::Five)
        .value("Six", MachEmu::ISR::Six)
        .value("Seven", MachEmu::ISR::Seven)
        .value("Load", MachEmu::ISR::Load)
        .value("Save", MachEmu::ISR::Save)
        .value("Quit", MachEmu::ISR::Quit)
        .value("NoInterrupt", MachEmu::ISR::NoInterrupt);

#ifdef ENABLE_MACHINE_HOLDER
    py::class_<MachEmu::MachineHolder>(MachEmu, "MakeMachine")
        .def(py::init<>())
        .def(py::init<const char*>())
        .def("OnLoad", &MachEmu::MachineHolder::OnLoad)
        .def("OnSave", &MachEmu::MachineHolder::OnSave)
        .def("Run", &MachEmu::MachineHolder::Run)
        .def("Save", &MachEmu::MachineHolder::Save)
        .def("SetClockResolution", &MachEmu::MachineHolder::SetClockResolution)
        .def("SetIoController", &MachEmu::MachineHolder::SetIoController)
        .def("SetMemoryController", &MachEmu::MachineHolder::SetMemoryController)
        .def("SetOptions", &MachEmu::MachineHolder::SetOptions)
        .def("WaitForCompletion", &MachEmu::MachineHolder::WaitForCompletion);
#else
    MachEmu.def("Make8080Machine", &MachEmu::Make8080Machine);

    py::class_<MachEmu::IMachine, std::shared_ptr<MachEmu::IMachine>>(MachEmu, "IMachine")
        .def("Run", &MachEmu::IMachine::Run)
        .def("SetClockResolution", &MachEmu::IMachine::SetClockResolution)
        .def("SetIoController", &MachEmu::IMachine::SetIoController);
        .def("SetMemoryController", &MachEmu::IMachine::SetMemoryController);
        .def("SetOptions", &MachEmu::IMachine::SetOptions)
        .def("WaitForCompletion", &MachEmu::IMachine::WaitForCompletion);
#endif

    py::class_<MachEmu::IController, MachEmu::ControllerPy>(MachEmu, "Controller")
        .def(py::init<>())
        .def("Read", &MachEmu::IController::Read)
        .def("Write", &MachEmu::IController::Write)
        .def("ServiceInterrupts", &MachEmu::IController::ServiceInterrupts)
        .def("Uuid", &MachEmu::IController::Uuid);
}
