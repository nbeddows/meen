import sys
import re
import unittest

from MachEmuPy import __version__
from MachEmuPy import ErrorCode
from MachEmuPy import MakeMachine

# relative path to Python controller test modules
sys.path.append('../../TestControllers/source')
# relative path to the C++ controller test module
sys.path.append('../../../bin/')

# import Python controller modules (a port of the c++ modules below)
# always use the c++ memory controller module for performance reasons, the python module is available strictly for demonstration purposes
#from MemoryController import MemoryController
from TestIoController import TestIoController
from CpmIoController import CpmIoController

# These c++ controller module versions are also available (CpmIoController currently not working)
# always use the c++ memory controller for performance reasons
from TestControllersPy import MemoryController
#from TestControllersPy import CpmIoController
#from TestControllersPy import TestIoController

class MachineTest(unittest.TestCase):
    def setUp(self):
        self.programsDir = '../../Programs/'
        self.memoryController = MemoryController()
        self.cpmIoController = CpmIoController(self.memoryController)
        self.testIoController = TestIoController()
        # lock the servicing of interrupts to the clock resolution
        self.machine = MakeMachine(r'{"cpu":"i8080","isrFreq":1,"runAsync":false}')
        err = self.machine.SetClockResolution(-1)
        self.assertEqual(err, ErrorCode.NoError)
        self.machine.SetIoController(self.testIoController)
        self.machine.SetMemoryController(self.memoryController)

    def test_Version(self):
        self.assertTrue(re.match(r'^(?P<major>0|[1-9]\d*)\.(?P<minor>0|[1-9]\d*)\.(?P<patch>0|[1-9]\d*)(?:-(?P<prerelease>(?:0|[1-9]\d*|\d*[a-zA-Z-][0-9a-zA-Z-]*)(?:\.(?:0|[1-9]\d*|\d*[a-zA-Z-][0-9a-zA-Z-]*))*))?(?:\+(?P<buildmetadata>[0-9a-zA-Z-]+(?:\.[0-9a-zA-Z-]+)*))?$', __version__))

    def test_SetNoIoController(self):
        with self.assertRaises(ValueError):
            self.machine.SetIoController(None)

    def test_SetNoMemoryController(self):
        with self.assertRaises(ValueError):
            self.machine.SetMemoryController(None)

    def test_SetCpuAfterConstruction(self):
        with self.assertRaises(RuntimeError):
            self.machine.SetOptions(r'{"cpu":"i8080"}')

    def test_MethodsThrowAfterRunCalled(self):
        self.memoryController.Load(self.programsDir + 'nopStart.bin', 0x0000)
        self.memoryController.Load(self.programsDir + 'nopEnd.bin', 0xC34F)
        self.machine.SetOptions(r'{"runAsync":true}')
        err = self.machine.SetClockResolution(25000000)
        self.assertEqual(err, ErrorCode.NoError)

        self.machine.Run(0)

        with self.assertRaises(RuntimeError):
        	self.machine.Run(0x100)

        with self.assertRaises(RuntimeError):
        	self.machine.SetClockResolution(50000000)

        with self.assertRaises(RuntimeError):
            self.machine.SetOptions(r'{"isrFreq":1}')

        with self.assertRaises(RuntimeError):
            self.machine.SetMemoryController(self.memoryController)

        with self.assertRaises(RuntimeError):
            self.machine.SetIoController(self.testIoController)
        
        self.machine.WaitForCompletion()

        # The machine has now stopped, all the following calls shouldn't throw

        err = self.machine.SetClockResolution(50000000)
        self.assertEqual(err, ErrorCode.NoError)

        err = self.machine.SetOptions(r'{"isrFreq":1}')
        self.assertEqual(err, ErrorCode.NoError)

        self.machine.SetMemoryController(self.memoryController)
        self.machine.SetIoController(self.testIoController)

    def test_BadClockResolution(self):
        err = self.machine.SetClockResolution(0)
        self.assertEqual(err, ErrorCode.ClockResolution)

    def RunTimed(self, runAsync):
        self.memoryController.Load(self.programsDir + 'nopStart.bin', 0x0000)
        self.memoryController.Load(self.programsDir + 'nopEnd.bin', 0xC34F)

        if runAsync == True:
            self.machine.SetOptions(r'{"runAsync":true}')

        # 60Hz clock
        err = self.machine.SetClockResolution(16666667)
        self.assertEqual(err, ErrorCode.NoError)

        nanos = 0
        iterations = 1

        for i in range(iterations):
            if runAsync == True:
                self.machine.Run(0)
                nanos += self.machine.WaitForCompletion()
            else:
                nanos += self.machine.Run(0)

        error = nanos / iterations - 1000000000
        self.assertTrue(error >= 0 and error <= 500000)

    def test_RunTimed(self):
        self.RunTimed(False)

    def test_RunTimedAsync(self):
        self.RunTimed(True)

    def test_8080Pre(self):
        self.memoryController.Load(self.programsDir + 'exitTest.bin', 0x0000)
        self.memoryController.Load(self.programsDir + 'bdosMsg.bin', 0x0005)
        self.memoryController.Load(self.programsDir + '8080PRE.COM', 0x0100)
        self.machine.SetIoController(self.cpmIoController)
        self.machine.Run(0x0100)
        self.assertIn('8080 Preliminary tests complete', self.cpmIoController.Message())

    def test_Tst8080(self):
        self.memoryController.Load(self.programsDir + 'exitTest.bin', 0x0000)
        self.memoryController.Load(self.programsDir + 'bdosMsg.bin', 0x0005)
        self.memoryController.Load(self.programsDir + 'TST8080.COM', 0x0100)
        self.machine.SetIoController(self.cpmIoController)
        self.machine.Run(0x0100)
        self.assertIn('CPU IS OPERATIONAL', self.cpmIoController.Message())

    # this will take a little while to complete
    def test_Cpu8080(self):
        self.memoryController.Load(self.programsDir + 'exitTest.bin', 0x0000)
        self.memoryController.Load(self.programsDir + 'bdosMsg.bin', 0x0005)
        self.memoryController.Load(self.programsDir + 'CPUTEST.COM', 0x0100)
        self.machine.SetIoController(self.cpmIoController)
        self.machine.Run(0x0100)
        self.assertIn('CPU TESTS OK', self.cpmIoController.Message())

    # this will take a long time to complete
    def test_8080Exm(self):
        self.memoryController.Load(self.programsDir + 'exitTest.bin', 0x0000)
        self.memoryController.Load(self.programsDir + 'bdosMsg.bin', 0x0005)
        self.memoryController.Load(self.programsDir + '8080EXM.COM', 0x0100)
        self.machine.SetIoController(self.cpmIoController)
        self.machine.Run(0x0100)
        self.assertNotIn('ERROR', self.cpmIoController.Message())

if __name__ == '__main__':
    unittest.main()