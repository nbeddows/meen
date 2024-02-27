#include <pybind11/pybind11.h>
#include "Controller/IController.h"

import CpmIoController;
import MemoryController;
import TestIoController;

namespace py = pybind11;

PYBIND11_MODULE(TestControllersPy, TestControllers)
{
    py::class_<MachEmu::MemoryController, MachEmu::IController>(TestControllers, "MemoryController")
        .def(py::init<>())
        .def("Clear", &MachEmu::MemoryController::Clear)
        .def("Load", &MachEmu::MemoryController::Load)
        .def("Read", &MachEmu::MemoryController::Read)
        .def("Write", &MachEmu::MemoryController::Write)
        .def("ServiceInterrupts", &MachEmu::MemoryController::ServiceInterrupts);

    py::class_<MachEmu::TestIoController, MachEmu::IController>(TestControllers, "TestIoController")
        .def(py::init<>())
        .def("Read", &MachEmu::TestIoController::Read)
        .def("Write", &MachEmu::TestIoController::Write)
        .def("ServiceInterrupts", &MachEmu::TestIoController::ServiceInterrupts);

    py::class_<MachEmu::CpmIoController, MachEmu::IController>(TestControllers, "CpmIoController")
        .def(py::init<const std::shared_ptr<MachEmu::IController>&>())
        .def("Message", &MachEmu::CpmIoController::Message)
        .def("Read", &MachEmu::CpmIoController::Read)
        .def("Write", &MachEmu::CpmIoController::Write)
        .def("ServiceInterrupts", &MachEmu::CpmIoController::ServiceInterrupts);
}