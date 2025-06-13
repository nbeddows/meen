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
        .value("ClockSamplingFreq", meen::errc::clock_sampling_freq)
        .value("Compressor", meen::errc::compressor)
        .value("Cpu", meen::errc::cpu)
        .value("Encoder", meen::errc::encoder)
        .value("IncompatibleRam", meen::errc::incompatible_ram)
        .value("IncompatibleRom", meen::errc::incompatible_rom)
        .value("IncompatibleUuid", meen::errc::incompatible_uuid)
        .value("InvalidArgument", meen::errc::invalid_argument)
        .value("IoController", meen::errc::io_controller)
        .value("JsonConfig", meen::errc::json_config)
        .value("JsonParse", meen::errc::json_parse)
        .value("MemoryController", meen::errc::memory_controller)
        .value("NotImplemented", meen::errc::not_implemented)
        .value("UnknownOption", meen::errc::unknown_option)
        .value("UriScheme", meen::errc::uri_scheme);

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
        .def("OnLoad", [](meen::IMachine& machine, std::function<std::string(meen::IController* ioController)>&& onLoad, std::function<meen::errc(meen::IController* ioController)>&& onLoadComplete)
        {
            if (onLoad)
            {
                return static_cast<meen::errc>(machine.OnLoad([ol = std::move(onLoad)](char* json, int* jsonLen, meen::IController* ioController)
                {   
                    auto loadState = ol(ioController);

                    if (loadState.size() > *jsonLen)
                    {
                        return meen::errc::invalid_argument;
                    }

                    *jsonLen = loadState.length();
                    strncpy(json, loadState.c_str(), *jsonLen);
                    return meen::errc::no_error;
                }, [olc = std::move(onLoadComplete)](meen::IController* ioController)
                {
                    return olc ? olc(ioController) : meen::errc::no_error;
                }).value());
            }
            else
            {
                return static_cast<meen::errc>(machine.OnLoad(nullptr, nullptr).value());
            }
        })
        .def("OnSave", [](meen::IMachine& machine, std::function<meen::errc(std::string&& json, meen::IController* ioController)>&& onSave)
        {
            if (onSave)
            {
                return static_cast<meen::errc>(machine.OnSave([os = std::move(onSave)](const char* json, meen::IController* ioController)
                {
                    return os(std::move(json), ioController);
                }).value());
            }
            else
            {
                return static_cast<meen::errc>(machine.OnSave(nullptr).value());
            }
        })
        .def("OnInit", [](meen::IMachine& machine, std::function<meen::errc(meen::IController* ioController)>&& onInit)
        {
            if (onInit)
            {
                return static_cast<meen::errc>(machine.OnInit([oi = std::move(onInit)](meen::IController* ioController)
                {
                    return oi(ioController);
                }).value());
            }
            else
            {
                return static_cast<meen::errc>(machine.OnInit(nullptr).value());
            }
        })
        .def("OnIdle", [](meen::IMachine& machine, std::function<bool(meen::IController* ioController)>&& onIdle)
        {
            if (onIdle)
            {
                return static_cast<meen::errc>(machine.OnIdle([oi = std::move(onIdle)](meen::IController* ioController)
                {
                    pybind11::gil_scoped_acquire gil{};
                    auto ret = oi(ioController);
                    pybind11::gil_scoped_release nogil{};
                    return ret;
                }).value());
            }
            else
            {
                return static_cast<meen::errc>(machine.OnIdle(nullptr).value());
            }
        })
        .def("OnError", [](meen::IMachine& machine, std::function<void(meen::errc ec, const char* fileName, const char* functionName, uint32_t line, uint32_t column, meen::IController* ioController)>&& onError)
        {
            if (onError)
            {
                return static_cast<meen::errc>(machine.OnError([oe = std::move(onError)](std::error_code ec, const char* fileName, const char* functionName, uint32_t line, uint32_t column, meen::IController* ioController)
                {
                    oe(static_cast<meen::errc>(ec.value()), fileName, functionName, line, column, ioController);
                }).value());
            }
            else
            {
                return static_cast<meen::errc>(machine.OnError(nullptr).value());
            }
        })
        .def("Run", [](meen::IMachine& machine)
        {
            pybind11::gil_scoped_release nogil{};
            return machine.Run().value_or(0);
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
        });

    py::class_<meen::IController, meen::ControllerPy>(meen, "Controller")
        .def(py::init<>())
        .def("Read", &meen::IController::Read)
        .def("Write", &meen::IController::Write)
        .def("GenerateInterrupt", &meen::IController::GenerateInterrupt)
        .def("Uuid", &meen::IController::Uuid);
}
