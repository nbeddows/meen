#ifndef MACHINEHOLDER_H
#define MACHINEHOLDER_H

#include "Machine/MachineFactory.h"

namespace MachEmu
{
    // Factory and Machine wrapper
    class MachineHolder final
    {
    private:
        std::unique_ptr<MachEmu::IMachine> machine_;
    public:
        MachineHolder();
        MachineHolder(const char* json);

        uint64_t Run(uint16_t offset);
        ErrorCode SetClockResolution(int64_t clockResolution);
        void SetIoController(MachEmu::IController* controller);
        void SetMemoryController(MachEmu::IController* controller);
        ErrorCode SetOptions(const char* options);
        uint64_t WaitForCompletion();
    };
} // namespace MachEmu

#endif // MACHINEHOLDER_H