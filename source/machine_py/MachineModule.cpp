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
        .value("Compressor", meen::errc::compressor)
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
        .def("OnLoad", [](meen::IMachine& machine, std::function<std::string()>&& onLoad)
        {
            return static_cast<meen::errc>(machine.OnLoad([ol = std::move(onLoad)](char* json, int* jsonLen)
            {
                // This static needs to be addressed ..... we could use a wrapper class for meen to store this ... don't really want to. :(
                static std::string jsonToLoad_;

                if(jsonLen == nullptr)
                {
                    return meen::errc::invalid_argument;
                }

                if (json == nullptr)
                {
                    jsonToLoad_ = ol();
                    *jsonLen = jsonToLoad_.length();
                }
                else
                {
                    if (jsonToLoad_.length() != *jsonLen - 1)
                    {
                        return meen::errc::invalid_argument;
                    }

                    strncpy(json, jsonToLoad_.c_str(), *jsonLen);
                }
                
                return meen::errc::no_error;
            }).value());            
        })
        .def("OnSave", [](meen::IMachine& machine, std::function<void(std::string&& json)>&& onSave)
        {
            if (onSave)
            {    
                return static_cast<meen::errc>(machine.OnSave([os = std::move(onSave)](const char* json)
                {
                    os(std::move(json));
                    return meen::errc::no_error;
                }).value());
            }
            else
            {
                return static_cast<meen::errc>(machine.OnSave(nullptr).value());
            }
        })
        .def("Run", [](meen::IMachine& machine)
        {
            return static_cast<meen::errc>(machine.Run().value());
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
