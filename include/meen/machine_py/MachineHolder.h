#ifndef MACHINEHOLDER_H
#define MACHINEHOLDER_H

#include "meen/Error.h"
#include "meen/MachineFactory.h"
#include "meen/machine/Machine.h"

namespace meen
{
    // Factory and Machine wrapper
    class MachineHolder final
    {
    private:
        std::unique_ptr<meen::IMachine> machine_;
        std::string json_;
    public:
        static MachineHolder Make8080Machine();

        MachineHolder(Cpu cpu);
        errc OnLoad(std::function<std::string()>&& onLoad);
        errc OnSave(std::function<void(std::string&&)>&& onSave);
        errc Run(uint16_t offset);
        errc SetIoController(meen::IController* controller);
        errc SetMemoryController(meen::IController* controller);
        errc SetOptions(const char* options);
        uint64_t WaitForCompletion();
    };
} // namespace meen

#endif // MACHINEHOLDER_H