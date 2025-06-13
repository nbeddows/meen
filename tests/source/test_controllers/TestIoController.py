from BaseIoController import BaseIoController
from meenPy import ISR

class TestIoController(BaseIoController):
    def __init__(self):
        super().__init__()
        self._deviceData = 0xAA
        self._lastTime = 0

    def Read(self, deviceNumber, controller):
        if deviceNumber == 0:
            return self._deviceData
        else:
            return 0x00

    def Write(self, deviceNumber, value, controller):
        if deviceNumber == 0:
            self._deviceData = value
        else:
            super().Write(deviceNumber, value, controller)

    def GenerateInterrupt(self, currTime, cycles, controller):
        isr = super().GenerateInterrupt(currTime, cycles, controller)

        if isr == ISR.NoInterrupt:
            t = currTime - self._lastTime

            if t >= 0:
                if t > 1000000000:
                    self._lastTime = currTime
                    isr = ISR.One
            else:
                self._lastTime = currTime

        return isr

    def Uuid(self):
        return [0xD8, 0x62, 0xFA, 0xBD, 0xDE, 0xDD, 0x47, 0xB7, 0x8C, 0x38, 0xD0, 0xDE, 0xB5, 0xCC, 0x45, 0xBE]
