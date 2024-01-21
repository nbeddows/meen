
### Introduction

MACHineEMUlator is a project which aims to define a simple framework for creating an emulated machine.

It represents a challenge I laid out for myself many years ago but has only got up and running
recently thanks to all the spare time we have all had due to the COVID-19 pandemic. It also gave
me the opportunity to explore the lastet features of the C++20 draft and make use of them where
applicable.

Project Goals: 

This list will expand as certain milestones are achieved.

1. Implement a complete Intel 8080 cpu emulator with passing individual unit tests for each instruction. It should also pass the standard Intel 8080 cpudiag tests which can be found online. **COMPLETE**

2. Implement a simple clock which will control the speed of the cpu and have the ability to have its resolution customised. **COMPLETE**

3. Implement an abstract controller interface which can be used to read and write data. The interface can then be used to create custom memory and io controllers which can be targeted towards specific programs and architectures. **COMPLETE**

4. Implement a basic system bus which can be used as 'lines' of communication between the cpu and various controllers. **COMPLETE**

5. Upgrade the 8080 to an 8085 (z80) complete with passing additional individual instruction unit tests. It should also pass the standard z80 zexall tests which can be found online. **NOT STARTED**

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
||8080PRE|PASS|
||CPUTEST|PASS|
||TST8080|PASS|

IMachine.h specifies the MachEmu interface and outlines the basic principles of operation.

### Compilation

Untar the mach-emu archive.

MachEmu uses CMake (3.28 is the minimum version required) for its build system and has been tested on both Window 10 and Ubuntu 23.10.

Open cmake-gui (feel free to use command line cmake, but the remainder of this readme will use cmake-gui). Set the source code text field to the mach-emu directory and binary directory for the build files.

Click configure and choose Visual Studio 17 for Windows or Unix Makefiles for Linux (if prompted to create the build directory, accept), then click generate.

##### Windows

The following image give a possible Windows CMake configuration (note that we don't use gmock and we don't require gtest installation so those options are turned off). Make sure that your install location is in your PATH environment variable otherwise MachEmu.dll will fail to load when the unit tests are run.

![Example Windows configuration](Docs/images/CMake(Windows).png)

MachEmu has been tested on visual studio and requires version 22 or newer (for c++ module support). Open the mach-emu visual studio solution, (depending on your install location you may need to open visual studio with admin privileges) set the configuration to Release and project to INSTALL, then build. Once this builds successfully you will be able to change your project to the machine unit tests and they should run successfully.

##### Linux

The following image give a possible Linux CMake configuration (note that we don't use gmock and we don't require gtest installation so those options are turned off). Also note that the required CXX compiler needs to be g++ 13 or greater (Clang is currently not suppoprted). If the gui output displays a different compiler you can open the root CMakeLists.txt an uncomment the following line #set(CMAKE_CXX_COMPILER g++-13)

![Example Linux configuration](Docs/images/CMake(Linux).png)

MachEmu has been tested with g++ and requires version 13.2 or newer (for module support) with GNU Make 4.3 or newer. Once CMake has finished change into the build directory and run make install. Depending on your install location you may need to run sudo make install. Once it completes the unit tests can be run from the relevant directory.

### Acknowledgements

Special thanks to the following sites:

https://altairclone.com/downloads/manuals/8080%20Programmers%20Manual.pdf<br>
https://altairclone.com/downloads/cpu_tests/<br>
https://caglrc.cc/~svo/i8080/