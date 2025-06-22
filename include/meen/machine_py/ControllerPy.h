#ifndef CONTROLLERPY_H
#define CONTROLLERPY_H

#include "meen/IController.h"

namespace meen
{
    struct ControllerPy final : public meen::IController
    {
        uint8_t Read(uint16_t address, IController* controller) final;
        void Write(uint16_t address, uint8_t value, IController* controller) final;
        meen::ISR GenerateInterrupt(uint64_t currTime, uint64_t cycles, IController* controller) final;
        std::array<uint8_t, 16> Uuid() const final;
    };
} // namespace meen

#endif // CONTROLLERPY_H