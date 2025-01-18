from BaseIoController import BaseIoController
from meenPy import ISR

class TestIoController(BaseIoController):
    def __init__(self):
        super().__init__()
        self.__deviceData = 0xAA
        self.__lastTime = 0

    def Read(self, deviceNumber, controller):
        # we are powering down, don't perform any spurious reads
        if self._isr == ISR.Quit:
            return 0x00

        if deviceNumber == 0:
            return self.__deviceData
        else:
            return 0x00

    def Write(self, deviceNumber, value, controller):
        # we are powering down, don't perform any spurious writes
        if self._isr == ISR.Quit:
            return

        if deviceNumber == 0:
            self.__deviceData = value
        else:
            super().Write(deviceNumber, value, controller)

    def ServiceInterrupts(self, currTime, cycles, controller):
        isr = super().ServiceInterrupts(currTime, cycles, controller)

        if isr == ISR.NoInterrupt:
            t = currTime - self.__lastTime

            if t >= 0:
                if t > 1000000000:
                    self.__lastTime = currTime
                    isr = ISR.One
            else:
                self.__lastTime = currTime

        return isr

    def Uuid(self):
        return [0xD8, 0x62, 0xFA, 0xBD, 0xDE, 0xDD, 0x47, 0xB7, 0x8C, 0x38, 0xD0, 0xDE, 0xB5, 0xCC, 0x45, 0xBE]
