#ifndef MACHINEHOLDER_H
#define MACHINEHOLDER_H

#include "meen/MachineFactory.h"
#include "meen/MEEN_Error.h"

namespace MachEmu
{
    // Factory and Machine wrapper
    class MachineHolder final
    {
    private:
        std::unique_ptr<MachEmu::IMachine> machine_;
        std::string json_;
    public:
        MachineHolder();
        MachineHolder(const char* json);

        void OnLoad(std::function<std::string()>&& onLoad);
        void OnSave(std::function<void(std::string&&)>&& onSave);
        uint64_t Run(uint16_t offset);
        std::string Save() const;
        errc SetClockResolution(int64_t clockResolution);
        void SetIoController(MachEmu::IController* controller);
        void SetMemoryController(MachEmu::IController* controller);
        errc SetOptions(const char* options);
        uint64_t WaitForCompletion();
    };
} // namespace MachEmu

#endif // MACHINEHOLDER_H