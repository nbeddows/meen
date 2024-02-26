from MachEmuPy import Controller
from MachEmuPy import ISR
import numpy as np

class MemoryController(Controller):
    def __init__(self):
        super().__init__()
        # 16 bit addressable memory
        self.__memory = np.zeros(65536, dtype=np.uint8)

    def Load(self, romFile, offset):
        file = open(romFile, "rb")
        data = np.fromfile(file, dtype=np.uint8)
        file.close()

        for i in range(data.size):
            self.__memory[offset] = data[i]
            offset += 1

    def Read(self, addr):
        return self.__memory[addr]

    def Write(self, addr, data):
        self.__memory[addr] = data

    def ServiceInterrupts(self, currTime, cycles):
        return ISR.NoInterrupt
        
