import json
import MachineTestDeps
import re
import sys
import unittest

from MachEmuPy import __version__
from MachEmuPy import ErrorCode
from MachEmuPy import MakeMachine

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
        # lock the servicing of interrupts to the clock resolution for performance reasons
        self.machine = MakeMachine(r'{"cpu":"i8080","clockResolution":-1,"isrFreq":1,"runAsync":false}')
        self.machine.SetIoController(self.testIoController)
        self.machine.SetMemoryController(self.memoryController)
        self.memoryController.Load(self.programsDir + 'exitTest.bin', 0x0000)
        self.memoryController.Load(self.programsDir + 'bdosMsg.bin', 0x0005)

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
    
    def test_NegativeISRFrequency(self):
        with self.assertRaises(ValueError):
            self.machine.SetOptions(r'{"isrFreq":-1.0}')

    def test_MethodsThrowAfterRunCalled(self):
        err = self.machine.SetOptions(r'{"clockResolution":25000000,"runAsync":true}')

        if err == ErrorCode.NotImplemented:
            return

        self.memoryController.Load(self.programsDir + 'nopStart.bin', 0x0004)
        self.memoryController.Load(self.programsDir + 'nopEnd.bin', 0xC353)
        self.machine.Run(0)

        with self.assertRaises(RuntimeError):
        	self.machine.Run(0x100)

        with self.assertRaises(RuntimeError):
            self.machine.SetOptions(r'{"isrFreq":1}')

        with self.assertRaises(RuntimeError):
            self.machine.SetMemoryController(self.memoryController)

        with self.assertRaises(RuntimeError):
            self.machine.SetIoController(self.testIoController)

        with self.assertRaises(RuntimeError):
            self.machine.OnSave(lambda x: print(x))
        
        self.machine.WaitForCompletion()

        # The machine has now stopped, all the following calls shouldn't throw

        err = self.machine.SetOptions(r'{"isrFreq":1}')
        self.assertEqual(err, ErrorCode.NoError)

        self.machine.SetMemoryController(self.memoryController)
        self.machine.SetIoController(self.testIoController)
        self.machine.OnSave(lambda x: print(x))

    def RunTimed(self, runAsync):
        if runAsync == True:
            err = self.machine.SetOptions(r'{"runAsync":true}')

            if err == ErrorCode.NotImplemented:
                return

        self.memoryController.Load(self.programsDir + 'nopStart.bin', 0x0004)
        self.memoryController.Load(self.programsDir + 'nopEnd.bin', 0xC353)

        # 60Hz clock
        err = self.machine.SetOptions(r'{"clockResolution":16666667}')
        self.assertEqual(err, ErrorCode.NoError)

        nanos = 0
        iterations = 1

        for i in range(iterations):
            if runAsync == True:
                self.machine.Run(0x0004)
                nanos += self.machine.WaitForCompletion()
            else:
                nanos += self.machine.Run(0x0004)

        error = nanos / iterations - 1000000000
        self.assertTrue(error >= 0 and error <= 500000)

    def test_RunTimed(self):
        self.RunTimed(False)

    def test_RunTimedAsync(self):
        self.RunTimed(True)

    def Load(self, runAsync):        
        if runAsync == True:
            err = self.machine.SetOptions(r'{"runAsync":true}')

            if err == ErrorCode.NotImplemented:
                return
        
        saveStates = []
        self.cpmIoController.SaveStateOn(3000)
        self.memoryController.Write(0x00FE, 0xD3)
        self.memoryController.Write(0x00FF, 0xFD)
        self.memoryController.Load(self.programsDir + 'TST8080.COM', 0x0100)
        err = self.machine.SetOptions(r'{"romOffset":0,"romSize":1727,"ramOffset":1727,"ramSize":256}')
        self.assertEqual(err, ErrorCode.NoError)
        self.machine.SetIoController(self.cpmIoController)
        self.machine.OnSave(lambda x: saveStates.append(x.rstrip('\0')))
        self.machine.OnLoad(lambda: saveStates[0])
        self.machine.Run(0x0100)

        if runAsync == True:
            self.machine.WaitForCompletion()

        self.machine.Run(0x00FE)

        if runAsync == True:
            self.machine.WaitForCompletion()

        self.assertIn('CPU IS OPERATIONAL', self.cpmIoController.Message())
        self.assertEqual(len(saveStates), 3)
        self.assertEqual(saveStates[0], r'{"cpu":{"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":19,"b":19,"c":0,"d":19,"e":0,"h":19,"l":0,"s":86},"pc":1236,"sp":1981},"memory":{"uuid":"zRjYZ92/TaqtWroc666wMQ==","rom":"JXg8/M+WvmCGVMmH7xr/0g==","ram":{"encoder":"base64","compressor":"zlib","size":256,"bytes":"eJwLZRhJQJqZn5mZ+TvTa6b7TJeZjjIxMAAAfY0E7w=="}}}')
        self.assertEqual(saveStates[1], saveStates[2])
        self.cpmIoController.SaveStateOn(-1)

    def test_OnLoad(self):
        self.Load(False)
    
    def test_OnLoadAsync(self):
        self.Load(True)

    def CheckMachineState(self, expected, actual):
        e = json.loads(expected)
        a = json.loads(actual.rstrip('\0'))        
        self.assertEqual(e, a['cpu'])

    def test_8080Pre(self):
        self.memoryController.Load(self.programsDir + '8080PRE.COM', 0x0100)
        self.machine.SetIoController(self.cpmIoController)
        self.machine.OnSave(lambda x: self.CheckMachineState(r'{"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":9,"d":3,"e":50,"h":1,"l":0,"s":86},"pc":2,"sp":1280}', x))
        self.machine.Run(0x0100)
        self.assertIn('8080 Preliminary tests complete', self.cpmIoController.Message())

    def test_Tst8080(self):
        self.memoryController.Load(self.programsDir + 'TST8080.COM', 0x0100)
        self.machine.SetIoController(self.cpmIoController)
        self.machine.OnSave(lambda x: self.CheckMachineState(r'{"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":170,"b":170,"c":9,"d":170,"e":170,"h":170,"l":170,"s":86},"pc":2,"sp":1981}', x))
        self.machine.Run(0x0100)
        self.assertIn('CPU IS OPERATIONAL', self.cpmIoController.Message())

    # this will take a little while to complete
    def test_Cpu8080(self):
        self.memoryController.Load(self.programsDir + 'CPUTEST.COM', 0x0100)
        self.machine.SetIoController(self.cpmIoController)
        self.machine.OnSave(lambda x: self.CheckMachineState(r'{"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":247,"d":4,"e":23,"h":0,"l":0,"s":70},"pc":2,"sp":12283}', x))
        self.machine.Run(0x0100)
        self.assertIn('CPU TESTS OK', self.cpmIoController.Message())

    # this will take a long time to complete
    def test_8080Exm(self):
        self.memoryController.Load(self.programsDir + '8080EXM.COM', 0x0100)
        self.machine.SetIoController(self.cpmIoController)
        self.machine.OnSave(lambda x: self.CheckMachineState(r'{"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":10,"c":9,"d":14,"e":30,"h":1,"l":109,"s":70},"pc":2,"sp":54137}', x))
        self.machine.Run(0x0100)
        self.assertNotIn('ERROR', self.cpmIoController.Message())

if __name__ == '__main__':
    unittest.main()