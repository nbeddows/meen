from meen_py import Controller
from meen_py import ISR

class BaseIoController(Controller):
    def __init__(self):
        super().__init__()
        self._saveCycleCount = -1
        self._load = False
        self._save = False
        self._powerOff = False

    def GenerateInterrupt(self, currTime, cycles, controller):
        isr = ISR.NoInterrupt

        # load takes precedence over save
        if self._load == True:
            isr = ISR.Load
            self._load = False
        elif self._save == True or self._saveCycleCount == cycles:
            isr = ISR.Save
            self._save = False
        elif self._powerOff == True:
            isr = ISR.Quit
            self._powerOff = False
        
        return isr
    
    def Write(self, port, value, controller):
        # don't clear an outstanding power off
        if self._powerOff == False:
            self._powerOff = port == 0xFF

        # don't clear an outstanding save
        if self._save == False:
            self._save = port == 0xFE

        # don't clear an outstanding load
        if self._load == False:
            self._load = port == 0xFD

    def SaveStateOn(self, cycleCount):
        self._saveCycleCount = cycleCount