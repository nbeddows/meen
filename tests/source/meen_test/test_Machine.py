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
        err = self.machine.SetOptions(r'{"isrFreq":0.02}')
        self.assertEqual(err, ErrorCode.NoError)

    def test_Version(self):
        self.assertTrue(re.match(r'^(?P<major>0|[1-9]\d*)\.(?P<minor>0|[1-9]\d*)\.(?P<patch>0|[1-9]\d*)(?:-(?P<prerelease>(?:0|[1-9]\d*|\d*[a-zA-Z-][0-9a-zA-Z-]*)(?:\.(?:0|[1-9]\d*|\d*[a-zA-Z-][0-9a-zA-Z-]*))*))?(?:\+(?P<buildmetadata>[0-9a-zA-Z-]+(?:\.[0-9a-zA-Z-]+)*))?$', __version__))

    def test_AttachNoIoController(self):
        err = self.machine.AttachIoController(None)
        self.assertEqual(err, ErrorCode.InvalidArgument)

    def test_AttachNoMemoryController(self):
        err = self.machine.AttachMemoryController(None)
        self.assertEqual(err, ErrorCode.InvalidArgument)

    def test_NegativeISRFrequency(self):
        err = self.machine.SetOptions(r'{"isrFreq":-1.0}')
        self.assertEqual(err, ErrorCode.JsonConfig)

    def test_MethodsErrorAfterRunCalled(self):
        # Write to the 'load device', the value doesn't matter (use 0)
        self.testIoController.Write(0xFD, 0, None)

        err = self.machine.OnLoad(lambda: r'{"cpu":{"pc":5},"memory":{"rom":{"block":[{"bytes":"file://' + self.programsDir + r'/exitTest.bin","offset":0},{"bytes":"file://' + self.programsDir + r'/nopStart.bin","offset":5},{"bytes":"file://' + self.programsDir + r'/nopEnd.bin","offset":50004}]}}}')
        self.assertEqual(err, ErrorCode.NoError)

        err = self.machine.SetOptions(r'{"clockResolution":25000000,"runAsync":true}')
        self.assertEqual(err, ErrorCode.NoError)

        # We aren't interested in saving, clear the onSave callback
        err = self.machine.OnSave(None)
        self.assertIn(err, [ErrorCode.NoError, ErrorCode.NotImplemented])
        
        err = self.machine.Run(0x0000)
        self.assertEqual(err, ErrorCode.NoError)

        err = self.machine.SetOptions(r'{"isrFreq":1}')
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

        err = self.machine.OnLoad(lambda: r'{"cpu":{"pc":5},"memory":{"rom":{"block":[{"bytes":"file://' + self.programsDir + r'/exitTest.bin","offset":0},{"bytes":"file://' + self.programsDir + r'/nopStart.bin","offset":5},{"bytes":"file://' + self.programsDir + r'/nopEnd.bin","offset":50004}]}}}')
        self.assertEqual(err, ErrorCode.NoError)

        if runAsync == True:
            err = self.machine.SetOptions(r'{"runAsync":true}')
            self.assertEqual(err, ErrorCode.NoError)

        # 60Hz clock
        err = self.machine.SetOptions(r'{"clockResolution":16666667}')
        self.assertEqual(err, ErrorCode.NoError)

        err = self.machine.Run(0x0000)
        self.assertEqual(err, ErrorCode.NoError)
        nanos = self.machine.WaitForCompletion()

        error = nanos - 1000000000
        self.assertTrue(error >= 0 and error <= 500000)

    def test_RunTimed(self):
        self.RunTimed(False)

    def test_RunTimedAsync(self):
        self.RunTimed(True)

    def Load(self, runAsync):
        saveStates = []

        def saveJson(json):
            saveStates.append(json.rstrip('\0'))
            return ErrorCode.NoError

        err = self.machine.OnSave(saveJson)
        
        if err == ErrorCode.NotImplemented:
            self.skipTest("Machine.OnSave is not supported")

        def loadJson():
            return saveStates[0]

        err = self.machine.OnLoad(loadJson)

        if err == ErrorCode.NotImplemented:
            self.skipTest("Machine.OnLoad is not supported")

        if runAsync == True:
            err = self.machine.SetOptions(r'{"runAsync":true,"loadAsync":false,"saveAsync":true}')
            self.assertEqual(err, ErrorCode.NoError)

        self.cpmIoController.SaveStateOn(3000)
        self.memoryController.Write(0x00FE, 0xD3, None)
        self.memoryController.Write(0x00FF, 0xFD, None)
        err = self.memoryController.Load(self.programsDir + 'TST8080.COM', 0x0100)
        self.assertEqual(err, ErrorCode.NoError)
        err = self.machine.AttachIoController(self.cpmIoController)
        self.assertEqual(err, ErrorCode.NoError)
        err = self.machine.Run(0x0100)
        self.assertEqual(err, ErrorCode.NoError)

        if runAsync == True:
            self.machine.WaitForCompletion()

        self.cpmIoController.SaveStateOn(-1)
        err = self.machine.Run(0x00FE)
        self.assertEqual(err, ErrorCode.NoError)

        if runAsync == True:
            self.machine.WaitForCompletion()

        self.assertIn('CPU IS OPERATIONAL', self.cpmIoController.Message())
        self.assertTrue(len(saveStates) == 2 or len(saveStates) == 3)
        self.assertEqual(saveStates[0], r'{"cpu":{"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":19,"b":19,"c":0,"d":19,"e":0,"h":19,"l":0,"s":86},"pc":1236,"sp":1981},"memory":{"uuid":"zRjYZ92/TaqtWroc666wMQ==","rom":"JXg8/M+WvmCGVMmH7xr/0g==","ram":{"encoder":"base64","compressor":"zlib","size":256,"bytes":"eJwLZRhJQJqZn5mZ+TvTa6b7TJeZjjIxMAAAfY0E7w=="}}}')
        self.assertEqual(saveStates[1], r'{"cpu":{"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":170,"b":170,"c":9,"d":170,"e":170,"h":170,"l":170,"s":86},"pc":2,"sp":1981},"memory":{"uuid":"zRjYZ92/TaqtWroc666wMQ==","rom":"JXg8/M+WvmCGVMmH7xr/0g==","ram":{"encoder":"base64","compressor":"zlib","size":256,"bytes":"eJw7w2ZczrCXnWFkAGlmfmZm5u9MYauCGFet2sXGwAAAYNgG1w=="}}}')

        if len(saveStates) == 3:
            self.assertEqual(saveStates[1], saveStates[2])

#    def test_OnLoad(self):
#        for i in range(50):
#            self.Load(False)

#    def test_OnLoadAsync(self):
#        for i in range(50):
#            self.Load(True)

class i8080Test(unittest.TestCase):
    def setUp(self):
        self.programsDir = MachineTestDeps.programsDir
        self.memoryController = MemoryController()
        self.cpmIoController = CpmIoController()
        self.machine = Make8080Machine()
        self.machine.AttachIoController(self.cpmIoController)
        self.machine.AttachMemoryController(self.memoryController)
        err = self.machine.SetOptions(r'{"isrFreq":0.02}')
        self.assertEqual(err, ErrorCode.NoError)
        self.saveTriggered = False

    def CheckMachineState(self, expected, actual):
        e = json.loads(expected)
        a = json.loads(actual.rstrip('\0'))
        self.assertEqual(e, a['cpu'])
        self.saveTriggered = True
#        ErrorCode.NoError

    def RunTestSuite(self, suiteName, expectedState, expectedMsg):
        # Write to the 'load device', the value doesn't matter (use 0)
        self.cpmIoController.Write(0xFD, 0, None)
        err = self.machine.OnLoad(lambda: r'{"cpu":{"pc":256},"memory":{"rom":{"block":[{"bytes":"file://' + self.programsDir + r'/exitTest.bin","offset":0},{"bytes":"file://' + self.programsDir + r'/bdosMsg.bin","offset":5},{"bytes":"file://' + self.programsDir + '/' + suiteName + r'","offset":256}]}}}')
        self.assertEqual(err, ErrorCode.NoError)
        err = self.machine.OnSave(lambda actualState: self.CheckMachineState(expectedState, actualState))
        self.assertIn(err, [ErrorCode.NoError, ErrorCode.NotImplemented])
        err = self.machine.Run(0x00)
        self.assertEqual(err, ErrorCode.NoError)
        self.assertTrue(self.saveTriggered)

        if suiteName == '8080EXM.COM':
            self.assertNotIn(expectedMsg, self.cpmIoController.Message())
        else:
            self.assertIn(expectedMsg, self.cpmIoController.Message())

    def test_8080Pre(self):
        self.RunTestSuite('8080PRE.COM', r'{"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":9,"d":3,"e":50,"h":1,"l":0,"s":86},"pc":5,"sp":1280}', '8080 Preliminary tests complete')

    def test_Tst8080(self):
        self.RunTestSuite('TST8080.COM', r'{"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":170,"b":170,"c":9,"d":170,"e":170,"h":170,"l":170,"s":86},"pc":5,"sp":1981}', 'CPU IS OPERATIONAL')

    def test_CpuTest(self):
        self.RunTestSuite('CPUTEST.COM', r'{"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":0,"c":247,"d":4,"e":23,"h":0,"l":0,"s":70},"pc":5,"sp":12283}', 'CPU TESTS OK')

    def test_8080Exm(self):
        self.RunTestSuite('8080EXM.COM', r'{"uuid":"O+hPH516S3ClRdnzSRL8rQ==","registers":{"a":0,"b":10,"c":9,"d":14,"e":30,"h":1,"l":109,"s":70},"pc":5,"sp":54137}', 'ERROR')

if __name__ == '__main__':
    unittest.main()