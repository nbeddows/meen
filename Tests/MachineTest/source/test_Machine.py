import re
import unittest

import MachEmuPy as MachEmu

class MachineTest(unittest.TestCase):
    def setUp(self):
        self.machine = MachEmu.Make8080Machine()
        err = self.machine.SetClockResolution(-1)
        self.assertEqual(err, MachEmu.ErrorCode.NoError)

    def test_Version(self):
        self.assertTrue(re.match(r'^(?P<major>0|[1-9]\d*)\.(?P<minor>0|[1-9]\d*)\.(?P<patch>0|[1-9]\d*)(?:-(?P<prerelease>(?:0|[1-9]\d*|\d*[a-zA-Z-][0-9a-zA-Z-]*)(?:\.(?:0|[1-9]\d*|\d*[a-zA-Z-][0-9a-zA-Z-]*))*))?(?:\+(?P<buildmetadata>[0-9a-zA-Z-]+(?:\.[0-9a-zA-Z-]+)*))?$', MachEmu.__version__))

    def test_SetNoIoController(self):
        with self.assertRaises(ValueError):
            self.machine.SetIoController(None)

    def test_SetNoMemoryController(self):
        with self.assertRaises(ValueError):
            self.machine.SetMemoryController(None)

    def test_BadClockResolution(self):
        err = self.machine.SetClockResolution(0)
        self.assertEqual(err, MachEmu.ErrorCode.ClockResolution)

if __name__ == '__main__':
    unittest.main()