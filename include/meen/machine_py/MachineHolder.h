#ifndef MACHINEHOLDER_H
#define MACHINEHOLDER_H

#include "meen/MachineFactory.h"
#include "meen/MEEN_Error.h"

namespace meen
{
    // Factory and Machine wrapper
    class MachineHolder final
    {
    private:
        std::unique_ptr<meen::IMachine> machine_;
        std::string json_;
    public:
        MachineHolder();
        MachineHolder(const char* json);

        errc OnLoad(std::function<std::string()>&& onLoad);
        errc OnSave(std::function<void(std::string&&)>&& onSave);
        errc Run(uint16_t offset);
        std::string Save() const;
        errc SetClockResolution(int64_t clockResolution);
        errc SetIoController(meen::IController* controller);
        errc SetMemoryController(meen::IController* controller);
        errc SetOptions(const char* options);
        uint64_t WaitForCompletion();
    };
} // namespace meen

#endif // MACHINEHOLDER_H