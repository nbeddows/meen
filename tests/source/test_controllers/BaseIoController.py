from meenPy import Controller
from meenPy import ISR

class BaseIoController(Controller):
    def __init__(self):
        super().__init__()
        self._isr = ISR.NoInterrupt
        self.__saveCycleCount = -1

    def ServiceInterrupts(self, currTime, cycles, controller):
        if cycles == self.__saveCycleCount:
            self._isr = ISR.Save

        isr = self._isr

        if self._isr == ISR.Quit or self._isr == ISR.Save or self._isr == ISR.Load:
            self._isr = ISR.NoInterrupt

        return isr
    
    def Write(self, port, value, controller):
        if port == 0xFF:
            self._isr = ISR.Quit
        elif port == 0xFE:
            self._isr = ISR.Save
        elif port == 0xFD:
            self._isr = ISR.Load

    def SaveStateOn(self, cycleCount):
        self.__saveCycleCount = cycleCount