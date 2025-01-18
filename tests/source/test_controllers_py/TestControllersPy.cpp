#include <pybind11/pybind11.h>

#include "meen/IController.h"
#include "test_controllers/CpmIoController.h"
#include "test_controllers/MemoryController.h"
#include "test_controllers/TestIoController.h"

namespace py = pybind11;

PYBIND11_MODULE(TestControllersPy, TestControllers)
{
    py::class_<meen::MemoryController, meen::IController>(TestControllers, "MemoryController")
        .def(py::init<>())
        .def("Clear", &meen::MemoryController::Clear)
        .def("Load", [](meen::MemoryController& controller, const char* romFilePath, uint16_t offset)
        {
            return controller.Load(romFilePath, offset).value();
        })
        .def("Read", &meen::MemoryController::Read)
        .def("Write", &meen::MemoryController::Write)
        .def("ServiceInterrupts", &meen::MemoryController::ServiceInterrupts);

    py::class_<meen::TestIoController, meen::IController>(TestControllers, "TestIoController")
        .def(py::init<>())
        .def("Read", &meen::TestIoController::Read)
        .def("Write", &meen::TestIoController::Write)
        .def("SaveStateOn", &meen::TestIoController::SaveStateOn)
        .def("ServiceInterrupts", &meen::TestIoController::ServiceInterrupts);

    py::class_<meen::CpmIoController, meen::IController>(TestControllers, "CpmIoController")
        .def(py::init<>())
        .def("Message", &meen::CpmIoController::Message)
        .def("Read", &meen::CpmIoController::Read)
        .def("Write", &meen::CpmIoController::Write)
        .def("SaveStateOn", &meen::CpmIoController::SaveStateOn)
        .def("ServiceInterrupts", &meen::CpmIoController::ServiceInterrupts);
}