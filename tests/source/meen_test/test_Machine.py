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

import json
import MachineTestDeps
import re
import sys
import unittest

from meenPy import __version__
from meenPy import ErrorCode
from meenPy import Make8080Machine

# Import Python controller modules (a port of the c++ modules are available below)
# Always use the c++ memory controller module for performance reasons, the python module is available strictly for demonstration purposes
#from MemoryController import MemoryController
from TestIoController import TestIoController
from CpmIoController import CpmIoController

# These c++ controller module versions are also available
# Always use the c++ memory controller for performance reasons
from TestControllersPy import MemoryController
#from TestControllersPy import CpmIoController
#from TestControllersPy import TestIoController

class MachineTest(unittest.TestCase):
    def setUp(self):
        self.loadIndex = 0
        self.programsDir = MachineTestDeps.programsDir
        self.memoryController = MemoryController()
        self.cpmIoController = CpmIoController()
        self.testIoController = TestIoController()
        self.machine = Make8080Machine()
        self.assertNotEqual(None, self.machine)
        err = self.machine.AttachIoController(self.testIoController)
        self.assertEqual(err, ErrorCode.NoError)
        err = self.machine.AttachMemoryController(self.memoryController)
        self.assertEqual(err, ErrorCode.NoError)
        # Using the default isrFreq setting of -1 (service interrupts at the end of each instruction) causes noticible performance issues, set it
        # to a number where on an i8080 running at 2Mhz, interrupts will be serviced every 40000 ticks.
        err = self.machine.SetOptions(r'json://{"isrFreq":60}')
        self.assertEqual(err, ErrorCode.NoError)
        # A base64 encoded code fragment that is loaded at address 0x0000 (for test suite compatibility) which saves the current machine state, powers off the machine, then halts the cpu.
        self.saveAndExit = 'base64://0/7T/3Y'
        # A base64 encoded code fragment that initialises the 'a' register to 10 and is loaded at address 0x0005. Used in conjunction with nopEnd to run the timing test
        self.nopStart = 'base64://Pgo='
        # A base64 encoded code fragment that decrements the 'a' register by 1, jumps to address 0x0005 if 'a' is non-zero else address 0x0000.
        self.nopEnd = 'base64://PcIHAMMAAA'
        # A base64 encoded code fragment that emulates cp/m bdos function 4 - raw console output.  
        self.bdosMsg = 'base64://9XnTAP4CyhEAetMBe9MC8ck='

    def test_Version(self):
        self.assertTrue(re.match(r'^(?P<major>0|[1-9]\d*)\.(?P<minor>0|[1-9]\d*)\.(?P<patch>0|[1-9]\d*)(?:-(?P<prerelease>(?:0|[1-9]\d*|\d*[a-zA-Z-][0-9a-zA-Z-]*)(?:\.(?:0|[1-9]\d*|\d*[a-zA-Z-][0-9a-zA-Z-]*))*))?(?:\+(?P<buildmetadata>[0-9a-zA-Z-]+(?:\.[0-9a-zA-Z-]+)*))?$', __version__))

    def test_AttachNoIoController(self):
        err = self.machine.AttachIoController(None)
        self.assertEqual(err, ErrorCode.InvalidArgument)

    def test_AttachNoMemoryController(self):
        err = self.machine.AttachMemoryController(None)
        self.assertEqual(err, ErrorCode.InvalidArgument)

    def test_NegativeISRFrequency(self):
        err = self.machine.SetOptions(r'json://{"isrFreq":-1.0}')
        self.assertEqual(err, ErrorCode.JsonConfig)

    def test_MethodsErrorAfterRunCalled(self):
        # Write to the 'load device', the value doesn't matter (use 0)
        self.testIoController.Write(0xFD, 0, None)

        err = self.machine.OnLoad(lambda: r'json://{"cpu":{"pc":5},"memory":{"rom":{"block":[{"bytes":"' + self.saveAndExit + r'","offset":0},{"bytes":"' + self.nopStart + r'","offset":5},{"bytes":"' + self.nopEnd + r'","offset":50004}]}}}')

        self.assertEqual(err, ErrorCode.NoError)

		#Sample the host clock 40 times per second, giving a meen clock tick a resolution of 25 milliseconds
        err = self.machine.SetOptions(r'json://{"clockSamplingFreq":40,"runAsync":true}')
        self.assertEqual(err, ErrorCode.NoError)

        # We aren't interested in saving, clear the onSave callback
        err = self.machine.OnSave(None)
        self.assertIn(err, [ErrorCode.NoError, ErrorCode.NotImplemented])
        
        err = self.machine.Run()
        self.assertEqual(err, ErrorCode.NoError)

        err = self.machine.SetOptions(r'json://{"isrFreq":1}')
        self.assertEqual(err, ErrorCode.Busy)
        err = self.machine.AttachMemoryController(self.memoryController)
        self.assertEqual(err, ErrorCode.Busy)
        err = self.machine.AttachIoController(self.testIoController)
        self.assertEqual(err, ErrorCode.Busy)
        err = self.machine.OnSave(lambda json: None)
        self.assertIn(err, [ErrorCode.Busy, ErrorCode.NotImplemented])

        self.machine.WaitForCompletion()

    def RunTimed(self, runAsync):
        # Write to the 'load device', the value doesn't matter (use 0)
        self.testIoController.Write(0xFD, 0, None)

        err = self.machine.OnLoad(lambda: r'json://{"cpu":{"pc":5},"memory":{"rom":{"block":[{"bytes":"' + self.saveAndExit + r'","offset":0},{"bytes":"' + self.nopStart + r'","offset":5},{"bytes":"' + self.nopEnd + r'","offset":50004}]}}}')
        self.assertEqual(err, ErrorCode.NoError)

        if runAsync == True:
            err = self.machine.SetOptions(r'json://{"runAsync":true}')
            self.assertEqual(err, ErrorCode.NoError)

        # Sample the host clock 40 times per second, giving a meen clock tick a resolution of 25 milliseconds
		# Service interrupts 60 times per meen cpu clock rate. For an i8080 running at 2Mhz, this would service interrupts every 40000 ticks.
        err = self.machine.SetOptions(r'json://{"clockSamplingFreq":40,"isrFreq":60}')
        self.assertEqual(err, ErrorCode.NoError)

        err = self.machine.Run()
        self.assertEqual(err, ErrorCode.NoError)
        nanos = self.machine.WaitForCompletion()

        error = nanos - 1000000000
        self.assertTrue(error >= 0 and error <= 500000)

    def test_RunTimed(self):
        self.RunTimed(False)

    def test_RunTimedAsync(self):
        self.RunTimed(True)

    def ReadCpmIoControllerBuffer(self):
        message = ''
        byte = 0x00; 

        while byte != 0x04: # ascii end of transmission
            byte = self.cpmIoController.Read(0, None)
		
            if byte != 0x04:
                message += chr(byte)

        return message

    def Load(self, runAsync):
        saveStates = []
        self.loadIndex = 0

        def saveJson(json):
            saveStates.append(json.rstrip('\0'))
            return ErrorCode.NoError

        err = self.machine.OnSave(saveJson)
        
        if err == ErrorCode.NotImplemented:
            self.skipTest("Machine.OnSave is not supported")

        def loadJson():
            jsonToLoad = ''

            match self.loadIndex:
                case 0:
                    jsonToLoad = r'json://{"cpu":{"pc":256},"memory":{"rom":{"block":[{"bytes":"' + self.saveAndExit + r'","offset":0},{"bytes":"' + self.bdosMsg + r'","offset":5},{"bytes":"file://' + self.programsDir + r'/TST8080.COM","offset":256,"size":1471}]}}}'
                case 1:
                    jsonToLoad = 'json://' + saveStates[0]
                case _:
                    pass

            self.loadIndex += 1
            return jsonToLoad

        err = self.machine.OnLoad(loadJson)

        if err == ErrorCode.NotImplemented:
            self.skipTest("Machine.OnLoad is not supported")

        if runAsync == True:
            err = self.machine.SetOptions(r'json://{"runAsync":true,"loadAsync":false,"saveAsync":true}')
            self.assertEqual(err, ErrorCode.NoError)

        # Need to set to 0 to catch all save interrupts
        err = self.machine.SetOptions(r'json://{"isrFreq":0}')
        self.assertEqual(err, ErrorCode.NoError)

        self.cpmIoController.Write(0xFD, 0, None)
        self.cpmIoController.SaveStateOn(3000)
        
        err = self.machine.AttachIoController(self.cpmIoController)
        self.assertEqual(err, ErrorCode.NoError)
        err = self.machine.Run()
        self.assertEqual(err, ErrorCode.NoError)

        time = self.machine.WaitForCompletion()
        self.assertNotEqual(time, 0)

        self.assertIn('CPU IS OPERATIONAL', self.ReadCpmIoControllerBuffer())

        self.cpmIoController.Write(0xFD, 0, None)
        self.cpmIoController.SaveStateOn(-1)
        err = self.machine.Run()
        self.assertEqual(err, ErrorCode.NoError)

        time = self.machine.WaitForCompletion()
        self.assertNotEqual(time, 0)

        self.assertIn('CPU IS OPERATIONAL', self.ReadCpmIoControllerBuffer())
        self.assertTrue(len(saveStates) == 2 or len(saveStates) == 3)
        self.assertEqual(saveStates[0], r'{"cpu":{"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":19,"b":19,"c":0,"d":19,"e":0,"h":19,"l":0,"s":86},"pc":1236,"sp":1981},"memory":{"uuid":"base64://zRjYZ92/TaqtWroc666wMQ==","rom":{"bytes":"base64://md5://BVt1f9Z97W/m34J/iH68cQ=="},"ram":{"size":64042,"bytes":"base64://zlib://eJztzlENgDAQBbDlnQAETBeSpwABCEDAfnHBktEqaGt/ca4OfKrXUVUzT+5cGVn9AQAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAGBXL4n+BO8="}}}')
        self.assertEqual(saveStates[1], r'{"cpu":{"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":170,"b":170,"c":9,"d":170,"e":170,"h":170,"l":170,"s":86},"pc":2,"sp":1981},"memory":{"uuid":"base64://zRjYZ92/TaqtWroc666wMQ==","rom":{"bytes":"base64://md5://BVt1f9Z97W/m34J/iH68cQ=="},"ram":{"size":64042,"bytes":"base64://zlib://eJztzkENgDAQALDBJeOJAGTghAdW8HQSSHAwP3xxwRJoFbSUv2h1Pco19W68ZIk5Iu5xz23IPGvvDwAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABf9QDDAAbX"}}}')

        if len(saveStates) == 3:
            self.assertEqual(saveStates[2], r'{"cpu":{"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":170,"b":170,"c":9,"d":170,"e":170,"h":170,"l":170,"s":86},"pc":2,"sp":1981},"memory":{"uuid":"base64://zRjYZ92/TaqtWroc666wMQ==","rom":{"bytes":"base64://md5://BVt1f9Z97W/m34J/iH68cQ=="},"ram":{"size":64042,"bytes":"base64://zlib://eJztzkENgDAQALDBJeOJAGTghAdW8HQSSHAwP3xxwRJoFbSUv2h1Pco19W68ZIk5Iu5xz23IPGvvDwAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABf9QDDAAbX"}}}')

    def test_OnLoad(self):
        for i in range(10):
            self.Load(False)

    def test_OnLoadAsync(self):
        for i in range(10):
            self.Load(True)

class i8080Test(unittest.TestCase):
    def setUp(self):
        self.programsDir = MachineTestDeps.programsDir
        self.memoryController = MemoryController()
        self.cpmIoController = CpmIoController()
        self.machine = Make8080Machine()
        self.machine.AttachIoController(self.cpmIoController)
        self.machine.AttachMemoryController(self.memoryController)
        # Using the default isrFreq setting of -1 (service interrupts at the end of each instruction) causes noticible performance issues, set it
        # to a number where on an i8080 running at 2Mhz, interrupts will be serviced evey 40000 ticks.
        err = self.machine.SetOptions(r'json://{"isrFreq":60}')
        self.assertEqual(err, ErrorCode.NoError)
        self.saveTriggered = False
        # A base64 encoded code fragment that is loaded at address 0x0000 (for test suite compatibility) which saves the current machine state, powers off the machine, then halts the cpu.
        self.saveAndExit = 'base64://0/7T/3Y'
        # A base64 encoded code fragment that emulates cp/m bdos function 4 - raw console output.  
        self.bdosMsg = 'base64://9XnTAP4CyhEAetMBe9MC8ck='

    def ReadCpmIoControllerBuffer(self):
        message = ''
        byte = 0x00; 

        while byte != 0x04: # ascii end of transmission
            byte = self.cpmIoController.Read(0, None)
		
            if byte != 0x04:
                message += chr(byte)

        return message

    def CheckMachineState(self, expected, actual):
        e = json.loads(expected)
        a = json.loads(actual.rstrip('\0'))
        self.assertEqual(e, a['cpu'])
        self.saveTriggered = True
#        ErrorCode.NoError

    def RunTestSuite(self, suiteName, expectedState, expectedMsg):
        # Write to the 'load device', the value doesn't matter (use 0)
        self.cpmIoController.Write(0xFD, 0, None)
        err = self.machine.OnLoad(lambda: r'json://{"cpu":{"pc":256},"memory":{"rom":{"block":[{"bytes":"' + self.saveAndExit + r'","offset":0},{"bytes":"' + self.bdosMsg + r'","offset":5},{"bytes":"file://' + self.programsDir + '/' + suiteName + r'","offset":256}]}}}')
        self.assertEqual(err, ErrorCode.NoError)
        err = self.machine.OnSave(lambda actualState: self.CheckMachineState(expectedState, actualState))
        self.assertIn(err, [ErrorCode.NoError, ErrorCode.NotImplemented])
        err = self.machine.Run()
        self.assertEqual(err, ErrorCode.NoError)
        self.assertTrue(self.saveTriggered or self.machine.OnSave(None) == ErrorCode.NotImplemented)

        if suiteName == '8080EXM.COM':
            self.assertNotIn(expectedMsg, self.ReadCpmIoControllerBuffer())
        else:
            self.assertIn(expectedMsg, self.ReadCpmIoControllerBuffer())

    def test_8080Pre(self):
        self.RunTestSuite('8080PRE.COM', r'{"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":9,"d":3,"e":50,"h":1,"l":0,"s":86},"pc":5,"sp":1280}', '8080 Preliminary tests complete')

    def test_Tst8080(self):
        self.RunTestSuite('TST8080.COM', r'{"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":170,"b":170,"c":9,"d":170,"e":170,"h":170,"l":170,"s":86},"pc":5,"sp":1981}', 'CPU IS OPERATIONAL')

    def test_CpuTest(self):
        self.RunTestSuite('CPUTEST.COM', r'{"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":247,"d":4,"e":23,"h":0,"l":0,"s":70},"pc":5,"sp":12283}', 'CPU TESTS OK')

    def test_8080Exm(self):
        self.RunTestSuite('8080EXM.COM', r'{"uuid":"base64://O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":10,"c":9,"d":14,"e":30,"h":1,"l":109,"s":70},"pc":5,"sp":54137}', 'ERROR')

if __name__ == '__main__':
    unittest.main()