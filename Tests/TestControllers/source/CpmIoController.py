from BaseIoController import BaseIoController
from mach_emuPy import ISR

class CpmIoController(BaseIoController):
    def __init__(self, memoryController):
        super().__init__()
        self.__memoryController = memoryController
        self.__message = ''
        self.__printMode = 0
        self.__addrHi = 0

    def Read(self, deviceNumber):
        return 0

    def Write(self, deviceNumber, value):
        match deviceNumber:
            case 0:
                self.__printMode = value
            case 1:
                self.__addrHi = value
            case 2:
                match self.__printMode:
                    case 9:
                        addr = (self.__addrHi << 8) | value
                        aChar = chr(self.__memoryController.Read(addr))

                        while aChar != '$':
                            self.__message += aChar
                            addr += 1
                            aChar = chr(self.__memoryController.Read(addr))
                    case 2:
                        self.__message += chr(value)
                    case _:
                        pass
            case _:
                super().Write(deviceNumber, value)

    def ServiceInterrupts(self, currTime, cycles):
        return super().ServiceInterrupts(currTime, cycles)

    def Message(self):
        return self.__message

    def Uuid(self):
        return [0x32, 0x8C, 0xCF, 0x78, 0x76, 0x1B, 0x48, 0xA4, 0x98, 0x2C, 0x1A, 0xAA, 0x5F, 0x14, 0x31, 0x24]