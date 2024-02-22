import MachEmuPy as MachEmu

class BaseIoController(MachEmu.Controller):
    def __init__(self):
        super().__init__()
        self.__isr = MachEmu.ISR.NoInterrupt

    def ServiceInterrupts(self, currTime, cycles):
        isr = self.__isr

        if self.__isr == MachEmu.ISR.Quit:
            self.__isr = MachEmu.ISR.NoInterrupt

        return isr
    
    def Write(self, port, value):
        if port == 0xFF:
            self.__isr = MachEmu.ISR.Quit