# Copyright (c) 2021-2025 Nicolas Beddows <nicolas.beddows@gmail.com>
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

from BaseIoController import BaseIoController
from meenPy import ISR

class CpmIoController(BaseIoController):
    def __init__(self):
        super().__init__()
        self._output = []
        self._printMode = 0
        self._addrHi = 0

    def Read(self, deviceNumber, controller):
        if len(self._output) > 0:
            return self._output.pop(0)
        else:
            return 0x04 # ascii end of transmission

    def Write(self, deviceNumber, value, controller):
        match deviceNumber:
            case 0:
                self._printMode = value
            case 1:
                self._addrHi = value
            case 2:
                match self._printMode:
                    case 9:
                        addr = (self._addrHi << 8) | value
                        aChar = controller.Read(addr, None)

                        while aChar != 0x24: #'$'
                            self._output.append(aChar)
                            addr += 1
                            aChar = controller.Read(addr, None)
                    case 2:
                        self._output.append(value)
                    case _:
                        pass
            case _:
                super().Write(deviceNumber, value, controller)

    def GenerateInterrupt(self, currTime, cycles, memoryController):
        return super().GenerateInterrupt(currTime, cycles, memoryController)

    def Uuid(self):
        return [0x32, 0x8C, 0xCF, 0x78, 0x76, 0x1B, 0x48, 0xA4, 0x98, 0x2C, 0x1A, 0xAA, 0x5F, 0x14, 0x31, 0x24]