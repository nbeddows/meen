2.0.0
* Updated the project layout for improved workflow.
* Added `std::error_code` support.
* Removed the use of exceptions (methods return std::error_code).
* Binary package now built with cpack.
* Added the Conan config option `with_save` to enable/disable
  save support. 
* Added support for ArduinoJson for baremetal.
* Added RP2040 microcontroller support.
* Code refactor for a minimum 2x performace boost.
* The ISR frequency is now honoured when no clock resolution
  is set.
* Compiler Id and version are now incorporated
  into the package name.
* Changed top level namespace from MachEmu to meen.
* `IMachine::Run` now returns `std::expected`.
* Removed WaitForCompletion from the public API`.
* Updated the minimum msvc version required in the README to 1706.
* Added Error.h with errc enum to compare meen error_code values.
* Clang is no longer officially supported.
* Removed the following IMachine API methods: `SetClockResolution`,
  `Save`, `GetState`.
* Factory methods are based on cpu type (removed `MakeMachine`
  in favour of `Make8080Machine`) and take no options.
* Removed the Load method from the unit test memory controller.
* The original `IMachine::OnLoad` and `IMachine::OnSave` callback
  parameter signatures have changed.
* Added an on completion handler parameter to the
  `IMachine::OnLoad`and `IMachine::OnSave` methods.
* Removed superfluous pybind machine holder.
* Replaced `IMachine` api methods `SetIoController` and
  `SetMemoryController` with `AttachIoController` and
  `AttachMemoryController` and added `DetachIoController`
  and `DetachMemoryController` to allow for the machine
  to take and relinquish controller ownership.
* Added support for the i8080 halt instruction.
* Removed the pc (program counter) parameter from
  `IMachine::Run`.
* Removed `IMachine::SetOptions` support for ram/rom.
* Tidied up the unit tests by removing the i8080 test
  files and using base64 encoded versions instead.
* More flexibility for the `IMachine::OnLoad` callback json
  allowing for more methods of loading program roms.
* Cpm test io controller's `Read` method has been implemented
  and it's `Message` method removed.
* Removed the config parameter `clockResolution`.
* Added the config parameter `clockSamplingFreq` which accepts
  a double value in Hertz.
* The meaning of the ISRFreq config parameter has changed,
  see the readme for further details.
* Added the Conan config option `with_base64` to enable/disable
  `base64://` uri scheme support.
* Added support for `scheme` and `directory` parameters to the
  memory["rom"] section of the machine load state format.
* Added the following `IMachine` API methods: `OnError`, `OnIdle`,
  `OnInit`.
* The cpu will be reset when no cpu configutation has been
  specified by the `IMachine::OnLoad` handler.
* The distribution documentation has changed from html to pdf.
* Renamed `IController::ServiceInterrupts`
  `IController::GenerateInterrupt`.

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