2.0.0
* Updated the project layout for improved workflow.
* Added std::error_code support.
* Removed the use of exceptions (methods return std::error_code).
* Binary package now built with cpack.
* Added Conan option `with_save` to enable/disable 
  load/save support. 
* Added support for ArduinoJson for baremetal.
* Added RP2040 support.
* Code refactor for a 2x performace boost.
* The ISR frequency is now honoured when no clock resolution
  is set.
* Compiler Id and version are now incorporated
  into the package name.
* Changed top level namespace from MachEmu to meen.

1.6.2 [24/07/24]
* Deprecated config options `ramOffset`, `ramSize`,
  `romOffset` and `romSize`.
* Added support for fragmented ram/rom for load/save
  files via the following config options:
  `ram:file:offset`, `ram:file:size`, `rom:block:offset`
  and `rom:block:size`.

1.6.1 [24/06/24]
* Disable running the unit tests in conanfile.py
  for foreign architectures.

1.6.0 [14/06/24]
* Perform all compatibility checks before
  attempting to restore the machine state.
* Added Clang support.
* Added Arm Linux (aarch64, armv7hf) GCC support.
* Added Conan 2.0 support (See README).
* Replace Utils/base64.hpp with Conan package base64.
* Replace Utils/md5.c with Conan package hash-library.
* Sdk target generates binary packages only.

1.5.1 [22/04/24]
* Fixed api compatibility break in 1.5.0.
* Drop load/save requests while a load/save
  is in progress.

1.5.0 [19/04/24]
* Added Machine interface method `OnLoad`.
* Added Machine interface method `OnSave`.
* Added Controller interface method `Uuid`.
* Deprecated `IMachine::Save`.
* Added cmake option `enableZlib`.
* Added `encoder` config option.
* Added `compressor` config option.
* Added `loadAsync` config option.
* Added `saveAsync` config option.
* Added `ramOffset` config option.
* Added `ramSize` config option.
* Added `romOffset` config option.
* Added `romSize` config option.
* Removed the use of c++20 modules.
* Enabled MeenPy for Linux.
* Implemented `runAsync` option for Linux.

1.4.0 [19/03/24]
* Added Version API method.
* Added Python module MeenPy.
* Added Python unit tests.
* Added factory method `MakeMachine`.
* Deprecated factory method `Make8080Machine`.
* Added json config file support.
* Added `ISRFreq` config option.
* Added `cpu` type config option.
* Added `runAsync` config option.
* Deprecated `IMachine::GetState`.
* Deprecated `IMachine::SetClockResolution`.
* Added Machine Interface method `Save`.
* Added `clockResolution` config option.

1.3.0 [29/01/24]
* Added CMake support.
  Tested with Visual Studio 22 (Windows 10) and
  gcc 13.2 (Ubuntu 23.10).
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
* The static library meen.lib has been changed to a dll.
* MachineFactory::MakeMachine is now exported from meen.dll.
* Added a SpaceInvaders solution file to the Windows directory of
  the sdk.
* Changed the directory structure of the sdk Windows directory to
  include both Release and Debug directories to house output files
  and machuemu dependencies.

0.2.0 [17/09/21]
* Added meen sdk with SpaceInvaders test app.
* Now outputting all dependant meen libraries as meen.lib so
  only one library needs to be linked to.

0.1.0 [10/09/21]
* Added IMachine and IController interfaces.
* Added Intel 8080 emulator complete with unit tests for each instruction.
* Added TestIoController, CpmIoController and DefaultMemoryController to
  the controller module to help with cpu unit tests.
* Initial release.