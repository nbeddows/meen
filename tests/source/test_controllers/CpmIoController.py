from BaseIoController import BaseIoController
from meenPy import ISR

class CpmIoController(BaseIoController):
    def __init__(self):
        super().__init__()
        self.__message = ''
        self.__printMode = 0
        self.__addrHi = 0

    def Read(self, deviceNumber, controller):
        return 0

    def Write(self, deviceNumber, value, controller):
        # we are powering down, don't perform any spurious writes
        if self._isr == ISR.Quit:
            return

        match deviceNumber:
            case 0:
                self.__printMode = value
            case 1:
                self.__addrHi = value
            case 2:
                match self.__printMode:
                    case 9:
                        addr = (self.__addrHi << 8) | value
                        aChar = chr(controller.Read(addr, None))

                        while aChar != '$':
                            self.__message += aChar
                            addr += 1
                            aChar = chr(controller.Read(addr, None))
                    case 2:
                        self.__message += chr(value)
                    case _:
                        pass
            case _:
                super().Write(deviceNumber, value, controller)

    def ServiceInterrupts(self, currTime, cycles, memoryController):
        return super().ServiceInterrupts(currTime, cycles, memoryController)

    def Message(self):
        return self.__message

    def Uuid(self):
        return [0x32, 0x8C, 0xCF, 0x78, 0x76, 0x1B, 0x48, 0xA4, 0x98, 0x2C, 0x1A, 0xAA, 0x5F, 0x14, 0x31, 0x24]