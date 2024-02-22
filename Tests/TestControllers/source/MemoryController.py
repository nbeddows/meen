from MachEmuPy import Controller
from MachEmuPy import ISR

class MemoryController(Controller):
    def __init__(self):
        super().__init__()
        # 16 bit addressable memory
        self.__memory = [0]*65536

    def Load(self, romFile, offset):
        file = open(romFile, "rb")

        data = file.read(1)

        while data:
            self.__memory[offset] = data
            data = file.read(1)
            offset += 1

        file.close()

    def Read(self, addr):
        return self.__memory[addr]

    def Write(self, addr, data):
        self.__memory[addr] = data

    def ServiceInterrupts(self, currTime, cycles):
        return ISR.NoInterrupt
        
