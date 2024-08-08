from mach_emuPy import Controller
from mach_emuPy import ISR

class BaseIoController(Controller):
    def __init__(self):
        super().__init__()
        self.__isr = ISR.NoInterrupt
        self.__saveCycleCount = -1

    def ServiceInterrupts(self, currTime, cycles):
        if cycles == self.__saveCycleCount:
            self.__isr = ISR.Save

        isr = self.__isr

        if self.__isr == ISR.Quit or self.__isr == ISR.Save or self.__isr == ISR.Load:
            self.__isr = ISR.NoInterrupt

        return isr
    
    def Write(self, port, value):
        if port == 0xFF:
            self.__isr = ISR.Quit
        elif port == 0xFE:
            self.__isr = ISR.Save
        elif port == 0xFD:
            self.__isr = ISR.Load

    def SaveStateOn(self, cycleCount):
        self.__saveCycleCount = cycleCount