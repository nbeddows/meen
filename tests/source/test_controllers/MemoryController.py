from meenPy import Controller
from meenPy import ISR
import numpy as np

class MemoryController(Controller):
    def __init__(self):
        super().__init__()
        # 16 bit addressable memory
        self.__memory = np.zeros(65536, dtype=np.uint8)

    def Read(self, addr):
        return self.__memory[addr]

    def Write(self, addr, data):
        self.__memory[addr] = data

    def ServiceInterrupts(self, currTime, cycles, ioController):
        return ISR.NoInterrupt
    
    def Uuid(self):
        return [0xCD, 0x18, 0xD8, 0x67, 0xDD, 0xBF, 0x4D, 0xAA, 0xAD, 0x5A, 0xBA, 0x1C, 0xEB, 0xAE, 0xB0, 0x31]
        
