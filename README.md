
### Introduction

MACHineEMUlator is a project which aims to define a simple framework for creating an emulated machine.

##### Motivation

I needed something simple and extensible to keep myself busy during all the spare time we had during the COVID-19 pandemic
and to allow myself to keep up with the latest c++ standards, learn about cpu emulation (an interest my mine) and to pick up
a bit of Python and other scripting languages where applicable.

##### Project Goals

This list will expand as certain milestones are achieved.

1. Implement a complete Intel 8080 cpu emulator with passing individual unit tests for each instruction. It should also pass the standard Intel 8080 cpudiag tests which can be found online. **COMPLETE**

2. Implement a simple clock which will control the speed of the cpu and have the ability to have its resolution customised. **COMPLETE**

3. Implement an abstract controller interface which can be used to read and write data. The interface can then be used to create custom memory and io controllers which can be targeted towards specific programs and architectures. **COMPLETE**

4. Implement a basic system bus which can be used as 'lines' of communication between the cpu and various controllers. **COMPLETE**

5. Implement a Zilog Z80 cpu emulator complete with passing additional individual instruction unit tests. It should also pass the standard z80 zexall tests which can be found online. **NOT STARTED**

6. Add a Python module which wraps the emulator C++ shared library complete with unit tests. **IN PROGRESS**

### Overview

Conceptually speaking, MachEmu can be represented by the following diagram:

![](Docs/images/MachineDiagram.png)

As can be seen from the diagram above MachEmu is represented by the inner machine containing a cpu and a clock used to regulate its speed. The speed the clock runs at is dictated by the cpu type, however the resolution of the clock can be externally manipulted, see IMachine::SetClockResolution.

The outer machine represents the inner machine customisation. For example, custom input/output may involve interacting with a keyboard or mouse, or some other proprietary peripheral, whereas custom memory maybe as simple as reading and writing to a block of locally allocated memory, a network socket or some other proprietary memory configuration, this all depends on the machine being built, see IMachine::SetIoControlller and IMachine::SetMemoryController.

Example code in this sdk is supplied in the form of unit tests. A test IO contoller library is supplied which gives implementations of both an IO controller and
a memory controller.

The following table displays the current defacto test suites that these unit tests run.

| Cpu   | Test             | Status | 
|:-----:|:----------------:|:------:|
| i8080 | 8080EXM          | PASS   |
|       | 8080PRE          | PASS   |
|       | CPUTEST          | PASS   |
|       | TST8080          | PASS   |

IMachine.h specifies the MachEmu interface and outlines the basic principles of operation.

### Compilation

##### Pre-requisites

The following development packages require installation:

[cmake](https://cmake.org/download/)<br>
[nlohmann_json](https://github.com/nlohmann/json/releases)<br>

When the python module is enabled, the following development packages require installtion:

[Python3](https://www.python.org/downloads/windows/)<br>
Python3 development - **Linux:** sudo apt install python3-dev **Windows:** available via the advanced options in the installer.<br>
[pybind11](https://github.com/pybind/pybind11)<br>
[numpy](https://github.com/numpy/numpy)<br>

##### Configuration

Untar the mach-emu archive.

MachEmu uses CMake (3.28 is the minimum version required) for its build system and has been tested on both Window 10 and Ubuntu 23.10.

Open cmake-gui (feel free to use command line cmake, but the remainder of this readme will use cmake-gui). Set the source code text field to the mach-emu directory and binary directory for the build files.

Click configure and choose Visual Studio 17 for Windows or Unix Makefiles for Linux (if prompted to create the build directory, accept), then click generate.

##### Windows

The following image give a possible Windows CMake configuration (note that we don't use gmock and we don't require gtest installation so those options are turned off). Make sure that your install location is in your PATH environment variable otherwise MachEmu.dll will fail to load when the unit tests are run.

![Example Windows configuration](Docs/images/CMake(Windows).png)

MachEmu has been tested on visual studio and requires version 22 or newer (for c++ module support). Open the mach-emu visual studio solution, (depending on your install location you may need to open visual studio with admin privileges) set the configuration to Release and project to INSTALL, then build. Once this builds successfully you will be able to change your project to the machine and controller unit tests and they should run successfully.

##### Linux

The following image give a possible Linux CMake configuration (note that we don't use gmock and we don't require gtest installation so those options are turned off). Also note that the required CXX compiler needs to be g++ 13 or greater (Clang is currently not supported). If the gui output displays a different compiler you can open the root CMakeLists.txt and uncomment the following line `set(CMAKE_CXX_COMPILER g++-13)`

![Example Linux configuration](Docs/images/CMake(Linux).png)

MachEmu has been tested with g++ and requires version 13.2 or newer (for module support) with GNU Make 4.3 or newer. Once CMake has finished change into the build directory and run make install. Depending on your install location you may need to run sudo make install. Once it completes the Machine unit tests can be found in Tests/MachineTest and the controller tests in Tests/ControllerTest.

Known issue:

Error copying file (if different) from "/home/user/mach-emu/Tests/TestControllers/gcm.cache/BaseIoController.gcm" to "/home/user/mach-emu/Tests/gcm.cache".

Solution:

Remove the binary directory, reconfigure and rebuild.

##### Python

When the enablePythonModule option is checked a MachEmu Python module will be built and installed in the same directory as the MachEmu shared library.

The MachuEmu module needs to be in the Python interpreter search path, this can be done via one of the following (amoungst others) methods:

1. Add the MachEmu lib install path to your PYTHONPATH environment variable: export PYTHONPATH=${mach-emu-install-dir}/lib
2. At run time via the Python sys module: sys.path.append(${mach-emu-install-dir}/lib)

### Acknowledgements

Special thanks to the following sites:

[i8080 Manual](https://altairclone.com/downloads/manuals/8080%20Programmers%20Manual.pdf)<br>
[CPU Tests](https://altairclone.com/downloads/cpu_tests/)<br>
[Pretty Assmebler](https://caglrc.cc/~svo/i8080/)