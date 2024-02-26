1.4.0
* Added Version API method.
* Added Python module MachEmuPy.
* Added Python unit tests.

1.3.0 [29/01/24]
* Added CMake support.
  Tested with Visual Studio 22 (Windows 10) and
  g++ 13.2 (Ubuntu 23.10).
* Moved Space Invaders to it's own repository.
* Removed the Windows specific sdk batch script
  (now building via CMake).
* Updated the sdk to include the unit tests
  (removed Space Invaders).
* Renamed the sdk project target from mach-emu-sdk
  to mach-emu.
* Sdk modules are now headers files for better
  platform compatibility.
* Added SetClockResolution to the IMachine interface.
* Updated documentation.
* Added version information to dll/so.

1.2.0 [01/12/23]
* Added Cppcheck static analysis.

1.1.0 [28/11/23]
* Added audio support to Space Invaders.
* Abstracted Space Invaders SDL IO from
  the base IO allowing for different
  Space Invaders IO implementations to
  be more easily added.
* Added MIT license.

1.0.0 [24/11/23]

* Added gtests for all official 8080 test suites.
* Fixed carry and auxillary carry flags for the add
  and Daa instructions which were causing the test
  suites to fail.
* Perform the required 270 degree rotation of the
  Space Invaders video memory for correct rendering.
* Added keyboard support for Space invaders.
* Removed co-routines for performance reasons.

0.3.2 [07/10/21]

* Added a co-routine helper module for easier co-routine
  handling.
* Fixed large memory leak caused by co routine handles
  not being deallocated.
* Added S(imple)D(irectMedia)L(ayer) support to the sdk
  example for video rendering.

0.2.2 [21/09/21]

* Added Doxygen generated html documentation to the sdk.

0.2.1 [18/09/21]

* The static library machemu.lib has been changed to a dll.
* MachineFactory::MakeMachine is now exported from machemu.dll.
* Added a SpaceInvaders solution file to the Windows directory of
  the sdk.
* Changed the directory structure of the sdk Windows directory to
  include both Release and Debug directories to house output files
  and machuemu dependencies.

0.2.0 [17/09/21]

* Added machemu sdk with SpaceInvaders test app.
* Now outputting all dependant machemu libraries as machemu.lib so
  only one library needs to be linked to.

0.1.0 [10/09/21]

* Added IMachine and IController interfaces.
* Added Intel 8080 emulator complete with unit tests for each instruction.
* Added TestIoController, CpmIoController and DefaultMemoryController to
  the controller module to help with cpu unit tests.
* Initial release.