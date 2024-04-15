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

        void OnLoad(std::function<std::string()>& onSave);
        void OnSave(std::function<void(std::string&& json)>& onSave);
        uint64_t Run(uint16_t offset);
        std::string Save() const;
        ErrorCode SetClockResolution(int64_t clockResolution);
        void SetIoController(MachEmu::IController* controller);
        void SetMemoryController(MachEmu::IController* controller);
        ErrorCode SetOptions(const char* options);
        uint64_t WaitForCompletion();
    };
} // namespace MachEmu

#endif // MACHINEHOLDER_H