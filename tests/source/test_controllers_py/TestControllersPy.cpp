/*
Copyright (c) 2021-2025 Nicolas Beddows <nicolas.beddows@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

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
        .def("Read", &meen::MemoryController::Read)
        .def("Write", &meen::MemoryController::Write)
        .def("GenerateInterrupt", &meen::MemoryController::GenerateInterrupt);

    py::class_<meen::TestIoController, meen::IController>(TestControllers, "TestIoController")
        .def(py::init<>())
        .def("Read", &meen::TestIoController::Read)
        .def("Write", &meen::TestIoController::Write)
        .def("SaveStateOn", &meen::TestIoController::SaveStateOn)
        .def("GenerateInterrupt", &meen::TestIoController::GenerateInterrupt);

    py::class_<meen::CpmIoController, meen::IController>(TestControllers, "CpmIoController")
        .def(py::init<>())
        .def("Read", &meen::CpmIoController::Read)
        .def("Write", &meen::CpmIoController::Write)
        .def("SaveStateOn", &meen::CpmIoController::SaveStateOn)
        .def("GenerateInterrupt", &meen::CpmIoController::GenerateInterrupt);
}