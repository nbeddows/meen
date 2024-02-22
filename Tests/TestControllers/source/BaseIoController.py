from MachEmuPy import Controller
from MachEmuPy import ISR


class BaseIoController(Controller):
    def __init__(self):
        super().__init__()
        self.__isr = ISR.NoInterrupt

    def ServiceInterrupts(self, currTime, cycles):
        isr = self.__isr

        if self.__isr == ISR.Quit:
            self.__isr = ISR.NoInterrupt

        return isr
    
    def Write(self, port, value):
        if port == 0xFF:
            self.__isr = ISR.Quit