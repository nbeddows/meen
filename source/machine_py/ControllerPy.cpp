#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "meen/machine_py/ControllerPy.h"

namespace meen
{
    uint8_t ControllerPy::Read(uint16_t address, IController* controller)
    {
        PYBIND11_OVERRIDE_PURE(
            uint8_t,            /* Return type */
            IController,        /* Parent class */
            Read,               /* Name of function in C++ (must match Python name) */
            address,            /* Argument(s) */
            controller
        );
    };

    void ControllerPy::Write(uint16_t address, uint8_t value, IController* controller)
    {
        PYBIND11_OVERRIDE_PURE(
            void,               /* Return type */
            IController,        /* Parent class */
            Write,              /* Name of function in C++ (must match Python name) */
            address,            /* Argument(s) */
            value,
            controller
        );
    };

    meen::ISR ControllerPy::GenerateInterrupt(uint64_t currTime, uint64_t cycles, IController* controller)
    {
        PYBIND11_OVERRIDE_PURE(
            meen::ISR,       /* Return type */
            IController,        /* Parent class */
            GenerateInterrupt,  /* Name of function in C++ (must match Python name) */
            currTime,           /* Argument(s) */
            cycles,
            controller
        );
    }

    std::array<uint8_t, 16> ControllerPy::Uuid() const
    {
        using Uint8Array16 = std::array<uint8_t, 16>;

        PYBIND11_OVERRIDE_PURE(
            Uint8Array16,       /* Return type */
            IController,        /* Parent class */
            Uuid                /* Name of function in C++ (must match Python name) */
        );
    }
}