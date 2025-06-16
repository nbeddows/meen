
### Introduction

Machine Emulator ENgine (MEEN) is a project which aims to define a simple framework for creating an emulated machine. It supports the registration of custom handlers to support such functionality as loading machine state, saving machine state, error handling and one shot initialisation.

##### Motivation

I needed something simple and extensible to allow myself to keep up with the latest C/C++ standards and frameworks, learn about cpu emulation (an interest my mine) and to pick up a bit of Python and other scripting languages where applicable. This is just one of many implementations out there, and while speed is not a motivating factor behind this project, optimisations will be considered if and when they are required.

##### Project Goals

This list will expand as certain milestones are achieved.

1. Implement a complete Intel 8080 cpu emulator with passing individual unit tests for each instruction. It should also pass the standard Intel 8080 cpudiag tests which can be found online. **COMPLETE**

2. Implement a simple clock which will control the speed of the cpu and have the ability to have its resolution customised. **COMPLETE**

3. Implement an abstract controller interface which can be used to read and write data. The interface can then be used to create custom memory and io controllers which can be targeted towards specific programs and architectures. **COMPLETE**

4. Implement a Zilog Z80 cpu emulator complete with passing additional individual instruction unit tests. It should also pass the standard z80 zexall tests which can be found online. **NOT STARTED**

5. Add a Python module which wraps the emulator C++ shared library complete with unit tests. **COMPLETE**

6. Add support for the RP2040 microcontroller. **COMPLETE**

### Overview

Conceptually speaking, MEEN can be represented by the following diagram:

![](docs/images/MachineDiagram.png)

As can be seen from the diagram above MEEN is represented by the inner machine containing a cpu and a clock used to regulate its speed. The speed the clock runs at is dictated by the cpu type, however, the frequency at which it samples the host clock can be externally manipulted, see configuration option `clockSamplingFreq`.

The outer machine represents the inner machine customisation. For example, custom input/output may involve interacting with a keyboard or mouse, or some other proprietary peripheral, whereas custom memory maybe as simple as reading and writing to a block of locally allocated memory, a network socket or some other proprietary memory configuration, this all depends on the machine being built, see `IMachine::AttachIoControlller` and `IMachine::AttachMemoryController`.

Example code in this framework is supplied in the form of unit tests. A test IO contoller library is supplied which gives implementations of both an IO controller and a memory controller.

The following table displays the current defacto test suites that these unit tests run.

| Cpu   | Test             | Status | 
|:-----:|:----------------:|:------:|
| i8080 | 8080EXM          | PASS   |
| ^     | 8080PRE          | PASS   |
| ^     | CPUTEST          | PASS   |
| ^     | TST8080          | PASS   |

`IMachine.h` specifies the MEEN API.<br>
`MachineFactory.h` specifies the MEEN library entry point.

### Compilation

MEEN uses [CMake (minimum version 3.23)](https://cmake.org/) for its build system, [Conan (minimum version 2.0)](https://conan.io/) for it's dependency package manager, Python3-dev for python module support,[cppcheck](http://cppcheck.net/) for static analysis and [Doxygen](https://www.doxygen.nl/index.html) and latex for documentation. Supported compilers are GCC (minimum version 12) and MSVC (minimum version 1706). Clang (minimum version 16) hasn't been tested for a while, therefore, it is no longer officially supported.

#### Pre-requisites

##### Linux (Ubuntu (24.04 at time of writing))

- [Install Conan](https://conan.io/downloads/)
- `sudo apt install cmake`
- `sudo apt install cppcheck` (if building a binary development package)
- `sudo apt install doxygen` (if building a binary development package)
- `sudo apt install texlive-latex-base`
- `sudo apt install texlive-latex-extra`
- [Install Emojis](https://www.doxygen.nl/dl/github_emojis.zip) (download and unzip in docs/images)
- `sudo apt install python3 python-is-python3 python3-dev` (if building the Python module)
- cross compilation:
  - armv7hf:
    - `sudo apt install gcc-arm-linux-gnueabihf g++-arm-linux-gnueabihf`
  - aarch64:
    - `sudo apt install gcc-aarch64-linux-gnu g++-aarch64-linux-gnu`
  - rp2040:
    - `sudo apt install gcc-arm-none-eabi libnewlib-arm-none-eabi build-essential libstdc++-arm-none-eabi-newlib`
    - `git clone https://github.com/raspberrypi/pico-sdk.git --branch 1.5.1`
    - `cd pico-sdk`
    - `git submodule update --init`
    - build the Raspberry Pi Pico SDK:
      - Conan and the Raspberry Pi Pico Sdk seem to have an issue with conflicting use of the cmake toolchain file which results in test programs not being able to be compiled during the conan build process as outlined [here](https://github.com/raspberrypi/pico-sdk/issues/1693).
      At this point we need to build the sdk so that we have the required tools pre-built so the Conan build process will succeed:
      **NOTE**: Conan will assume that the build tools are located in the `build` directory, **do not** use a different directory name.
        - `mkdir build`
        - `cd build`
        - `cmake ..`
        - `make`
    - Set the Raspberry Pi Pico SDK Path:
        - `export PICO_SDK_PATH=${PATH_TO_PICO_SDK}`
        To avoid having to export it on every session, add it to the end of your .bashrc file instead:
        - `nano ~/.bashrc`
        - `export PICO_SDK_PATH=${PATH_TO_PICO_SDK}`
	- save, close and re-open shell.

##### Windows (10)

- [Install Conan](https://conan.io/downloads/)
- [Install CMake build system](https://cmake.org/download/).
- [Install CppCheck static analysis](http://cppcheck.net/) (if building a binary development package)
- [Install Doxygen](https://www.doxygen.nl/download.html) (if building a binary development package)
- [Install Tex Live](https://www.tug.org/texlive/windows.html)
- [Install Emojis](https://www.doxygen.nl/dl/github_emojis.zip) (download and unzip in docs/images)
- [Install Python3](https://www.python.org/downloads/windows/) (if building the Python module)
- Install `python3-dev`: available via the advanced options in the Python3 installer (if building the Python module)

#### Configuration

**1.** Install the supported MEEN Conan configurations (v0.1.0) (if not done so already):
- `conan config install -sf profiles -tf profiles `<br>
  `https://github.com/nbeddows/meen-conan-config.git --args "--branch v0.1.0"`

**2.** The installed profiles may need to be tweaked depending on your environment.

**3.** Install dependencies:
- Windows x86_64 build and host: `conan install . --build=missing --output-folder=output --profile:all=Windows-x86_64-msvc-193`
- Windows x86_64 build and host with unit tests: `conan install . --build=missing --output-folder=output --profile:all=Windows-x86_64-msvc-193-gtest`
- Linux x86_64 build and host: `conan install . --build=missing --profile:all=Linux-x86_64-gcc-13`
- Linux x86_64 build and host with unit tests: `conan install . --build=missing --profile:all=Linux-x86_64-gcc-13-gtest`
- Linux x86_64 build, Linux armv7hf host: `conan install . --build=missing -profile:build=Linux-x86_64-gcc-13 -profile:host=Linux-armv7hf-gcc-13`
- Linux x86_64 build, Linux armv7hf host with unit tests: `conan install . --build=missing -profile:build=Linux-x86_64-gcc-13 -profile:host=Linux-armv7hf-gcc-13-gtest`
- Linux x86_64 build, Linux aarch64 host: `conan install . --build=missing -profile:build=Linux-x86_64-gcc-13 -profile:host=Linux-armv8-gcc-13`
- Linux x86_64 build, Linux aarch64 host with unit tests: `conan install . --build=missing -profile:build=Linux-x86_64-gcc-13 -profile:host=Linux-armv8-gcc-13-gtest`
- Linux x86_64 build, RP2040 microcontroller (baremetal armv6-m) host: `conan install . --build=missing -profile:build=Linux-x86_64-gcc-13 -profile:host=rp2040-armv6-gcc-13`
- Linux x86_64 build, RP2040 microcontroller (baremetal armv6-m) host with unit tests: `conan install . --build=missing -profile:build=Linux-x86_64-gcc-13 -profile:host=rp2040-armv6-gcc-13-unity`<br>

**NOTE**: when performing a cross compile using a host profile you must install the requisite toolchain of the target architecture (see pre-requisites).<br>
**NOTE**: when using multi configuration generators (MSVC for example) we specify an output folder (--output-folder) so the directory depth remains constant with single configuation generators.

The following additional install options are supported:
- enable/disable python module support: `--options=with_python=[True|False(default)]` (Option not available on arm platforms)
- enable/disable zlib support: `--options=with_zlib=[True(default)|False]` (Options not available on embedded platofrms)

The following will enable python and disable zlib: `conan install . --build=missing --profile:all=Windows-x86_64-msvc-193 --options=with_python=True --options=with_zlib=False`

The following dependent packages will be (compiled if required and) installed based on the supplied options:

- `arduinojson`: for parsing machine configuration options  (baremetal).
- `base64`: for base64 coding.
- `gtest`: for running the MEEN unit tests.
- `hash-library`: for md5 hashing.
- `nlohmann_json`: for parsing machine configuration options.
- `pybind`: for creating Python C++ bindings.
- `unity`: for running the MEEN unit tests (baremetal).
- `zlib`: for memory (de)compression when loading and saving files.<br>

You can override the default build configuration to Debug (or MinRelSize or RelWithDebInfo) by overriding the build_type setting: `conan install . --build=missing --profile:all=Linux-x86_64-gcc-13 --settings=build_type=Debug`.

You can also compile the dependent zlib library statically if required by overriding the shared option: `conan install . --build=missing --profile:all=Linux-x86_64-gcc-13 --options=zlib/*:shared=False`.

**4.** Run cmake to configure and generate the build system.

- Multi configuration generators (MSVC for example): `cmake --preset conan-default [-Wno-dev]`
- Single configuration generators (make for example): `cmake --preset conan-release [-Wno-dev]`<br>
A Debug preset (or MinRelSize or RelWithDebugInfo) can be used if the said build_type was used during the previous step: `cmake --preset conan-debug`

**NOTE**: the options supported during the install step can also be enabled/disabled here if required:
- Disable zlib support: `cmake --preset conan-default -D enable_zlib=OFF`
- Enable the Python module: `cmake --preset conan-default -D enable_python_module=ON` (Unsupported on arm, CMake will fail)

**5.** Run cmake to compile MEEN: `cmake --build --preset conan-release`<br>

The presets of `conan-debug`, `conan-minsizerel` and `conan-relwithdebinfo` can also be used as long as they have been configured in the previous steps.

**NOTE**: when cross compiling the default build directory may need to be removed if any build conflicts occur: `rm -rf build`. Go to Step 3.

**6.** Run the unit tests:

**C++ - Linux/Windows (x86_64)**:
- `artifacts/Release/x86_64/bin/meen_test tests/programs/ [--gtest_filter=${gtest_filter}]`

**C++ - Linux (armv7hf, armv8):**<br>

When running a cross compiled build the binaries need to be uploaded to the host machine before they can be executed.
1. Create an Arm Linux binary distribution: See building a binary development package. 
2. Copy the distribution to the arm machine: `scp build/Release/meen-v1.5.1-Linux-armv7hf-bin.tar.gz ${user}@raspberrypi:meen-v1.5.1.tar.gz`.
3. Ssh into the arm machine: `ssh ${user}@raspberrypi`.
4. Extract the MEEN archive copied over via scp: `tar -xzf meen-v1.5.1.tar.gz`.
5. Change directory to meen: `cd meen`.
6. Run the unit tests: `./run-meen-unit-tests.sh [--gtest_filter ${gtest_filter}]`.<br>

**C++ - RP2040 (armv6-m)**

When running a cross compiled build the binaries need to be uploaded to the host machine before they can be executed.
This example will assume you are deploying the UF2 file from a Raspberry Pi.
1. Create an Arm Linux binary distribution: see building a binary development package.
2. Copy the distribution to the arm machine: `scp build/MinSizeRel/meen-v1.6.2-Linux-6.2.0-39-generic-armv6-bin.tar.gz ${user}@raspberrypi:meen-v1.6.2.tar.gz`.
3. Ssh into the arm machine: `ssh ${user}@raspberrypi`.
4. Extract the MEEN archive copied over via scp: `tar -xzf meen-v1.6.2.tar.gz`.
5. Hold down the `bootsel` button on the pico and plug in the usb cable into the usb port of the Raspberry Pi then release the `bootsel` button.
6. Echo the attached `/dev` device (this should show up as `sdb1` for example): `dmesg | tail`.
7. Create a mount point (if not done already): `sudo mkdir /mnt/pico`.
8. Mount the device: `sudo mount /dev/sdb1 /mnt/pico`. Run `ls /mnt/pico` to confirm it mounted.
9. Copy the uf2 image to the pico: `cp meen-v1.6.2-Linux-6.2.0-39-generic-armv6-bin/bin/meen_test.uf2 /mnt/pico`
10. You should see a new device `ttyACM0`: `ls /dev` to confirm.
11. Install minicom (if not done already): `sudo apt install minicom`.
12. Run minicom to see test output: `minicom -b 115200 -o -D /dev/ttyACM0`<br>
    You should see the test output running at 1 second intervals.
13. Quit minicom once done: `ctrl-a, x, enter`
14. Unmount the device: `sudo umount /mnt/pico`.

**Python**:
- `tests\source\meen_test\test_Machine.py -v [-k ${python_filter}]`

**Note**: for the C++ unit tests the command line option --gtest_filter can be used to run a subset of the tests and under Python the -k option can be used for the same effect.
- `artifacts\Release\x86_64\bin\meen_test --gtest_filter=*:-*8080*:*CpuTest*`: run all tests except the i8080 test suites.
- `tests\source\meen_test\test_Machine.py -v -k MachineTest`: run all tests except the i8080 test suites.

The location of the test programs directory can be overridden if required: `artifacts/Release/x86_64/bin/meen_test ${test/programs/directory/}`.

#### Building a binary development package

MEEN supports the building of standalone binary development packages. The motivation behind this is to have a package with minimal build dependencies (doesn't enforce the user of the package to use Conan and CMake for example). This allows the user to integrate the package into other environments where such dependencies may not be available.

Once the configuration step is complete a binary development tgz package can be built with the following command:
- `cmake --build --preset conan-release --target=package`

The package will be located in `build/Release/` with a name similar to the following depending on the platform it was built on:
- `meen-v1.5.1-Windows-10.0.19042-x86_64-bin.tar.gz`

CPack can be used directly rather than the package target if finer control is required:
- `cpack --config build\CPackConfig.cmake -C ${build_type} -G 7Z`

**NOTE**: the underlying package generator used to build the package (in this case `7zip`) must be installed otherwise the preceeding command will fail.

Run `cpack --help` for a list available generators.

The final package can be stripped by running the meen_strip_pkg target (defined only for platforms that support strip):
- `cmake --build --preset conan-release --target=meen_strip_pkg`

When the package has been built with unit tests enabled it will contain a script called `run-meen-unit-tests` which can be used to test the development package:
- `./run-meen-unit-tests.sh [--gtest_filter ${gtest_filter}] [--python_filter ${python_filter}]`
- `start run-meen-unit-tests.bat [--gtest_filter ${gtest_filter}] [--python_filter ${python_filter}]`

**NOTE**: the package will not contain Python units tests if MEEN was not configured with the python module enabled.

#### Export a Conan package

MEEN can be exported as a package to the local Conan cache so it can be consumed by other Conan based projects on the same machine. It supports the same options as discussed in step 3 of the configuration section.

The following additional options are supported:
- disable running the exported package tests: `--test_folder=""`
- enable/disable the unit tests for the i8080 suites: `--options=with_i8080_test_suites=[True|False(default)]` 

**NOTE**: a pre-requisite of exporting the package is the running of the unit tests (unless disabled). The export process will halt if the unit tests fail.

Example command lines:
1. `conan create . --build=missing --profile:all=Linux-x86_64-gcc-13-gtest`: build the MEEN package, run the unit tests, export it to the conan cache and then run a basic test to confirm that the exported package can be used.
2. `conan create . --build=missing --profile:all=Linux-x86_64-gcc-13-gtest --options=with_python=True`: same as 1 but will run the python unit tests, then run a basic test to confirm that the python module in the exported package can also be used.
3. `conan create . --build=missing --profile:all=Linux-x86_64-gcc-13-gtest --options=with_zlib=False`: same as 1 but will disable zlib support.
4. `conan create . --build=missing --profile:all=Linux-x86_64-gcc-13-gtest --test-folder=""`: same as 1 but will not run the basic package tests (not recommended).
5. `conan create . --build=missing --profile:all=Linux-x86_64-gcc-13`: same as 1 but will skip running the unit tests.
6. `conan create . --build=missing --profile:all=Linux-x86_64-gcc-13-gtest --options=with_i8080_test_suites=True`: same as 1 but will run the i8080 test suites.

#### Upload a Conan package

The package created in the previous section can be [uploaded to a Conan server](https://docs.conan.io/2/tutorial/conan_repositories/setting_up_conan_remotes/artifactory/artifactory_ce_cpp.html) so it can be consumed by other Conan based projects on other machines.

Example command lines (once the artifactory server has been installed and is running):
1. `conan remote add artifactory`<br>
    `http://<server-ip>:8081/artifactory/api/conan/conan-local`: add the package server to the list of Conan remotes.
2. `conan remote login artifactory <user> -p <password>`: login to the artifactory server so commands can be issued.
3. `conan remote list`: list the remotes to ensure that it has been added.
4. `conan upload meen -r=artifactory`: upload the MEEN package to the remote.
5. `conan search meen -r=artifactory`: search the remote to ensure that it was uploaded.
6. `conan remove meen -r=artifactory`: remove the MEEN package from the remote.
7. `conan remote remove artfactory`: remove the artifactory remote from the list of remotes.

### Basic principles of operation

Unless specifically stated in the `IMachine.h` api and/or `MachineFactory.h` factory, there is
no guarantee in regards to thread safety. All methods should be called from the same thread.

The Python and C++ APIs are identical except the following caveats:

- The Python API does not have a DetachMemmory/IO controller method. This is due to the fact that one can not
move memory from Python into C++. Attaching memory from Python does not move the memory but rather provides a
pointer to the memory. Due to this fact one must take care when accessing the controllers from Python. The
controllers **must not** be accessed while the machine is running. All API methods return ErrorCode.Busy when
the machine is running, therefore if you are unsure about the running state you can call the AttachIoController
method with `None`, if the method returns ErrorCode.Busy then the machine is currently running, otherwise it
will return ErrorCode.InvalidArgument and the machine is not running.

- The Python machine api method `OnLoad` returns the json machine state and accepts no parameters.

The following C++ code snippet gives an example of how a machine can be instantiated, configured and executed.
The Python API is identical (taking into account the caveats listed above).<br>

```cpp
// Create a synchronous i8080 machine running as fast as possible
auto machine = Make8080Machine();

// Register a method for error notification.
// All api methods return error codes where appropriate, however, this is the only mechanism available to
// obtain ALL generated errors with more detail.
// Note that this method may be called from a different thread if the runAsync/loadAsync/saveAsync
// configuration options have been specified. 
auto err = machine->OnError([](std::error_code ec, const char* fileName, const char* functionName, int line, int column, IController* ioController)
{
  // Handle the error, for demonstration purposes, print it to the console.
  std::cout << std::format("{}({}:{}) `{}`: {}", fileName, line, column, functionName, ec.message()) << std::endl;
});

// The return code of the OnError method must be checked as it may fail to register, therefore it won't be
// picked up in your OnError handler.
if (err)
{
  std::cout << "Failed to register the OnError method: " << err.message() << std::endl;
  return 0;
}

// Register a method whose code will be invoked exactly once during the initial call to IMachine::Run.
// Note that this can be called from a different thread if the runAsync configuration option
// has been specifed.
machine->OnInit([](IController* ioController)
{
  // perform once off initialisation tasks here.
});

// Register a handler that can load memory into rom. This can also be used to load json
// generated by the OnSave handler below.
// Note that this can be called from a different thread if the runAsync/loadAsync configuration options
// have been specifed.
machine->OnLoad([](char* json, int* jsonLength, IController* ioController)
{
  // Typically, the on load logic would be handled by your custom io controller:
  // ioController->LoadRom(json, jsonLen), but is left here for simplicity.

  // The machine state to load in json format: load all bytes from the rom myProgram.com from
  // the current directory into memory at address 0x0000 and start executing the rom from address 256.
  // Note the use of uri schemes to determine the type of resource to load (an ommitted scheme defaults
  // to json://).
  auto jsonToLoad = R"(json://{"cpu":{"pc":256},"memory":{"rom:"{"bytes":"file://myProgram.com"}}})"sv;
  // auto jsonToLoad = "file://myProgramSave.json"sv;
    
  // When the length of the json to load is greater than *jsonLen, the config option maxLoadStateLen
  // needs to be increased.
  if (jsonToLoad.length() > *jsonLength)
  {
    return errc::invalid_argument;
  }

  std::copy_n(json, jsonToLoad.length(), jsonToLoad.begin());
  // Write the final length of the loaded program
  *jsonLen = jsonToLoad.length();
  return errc::no_error;
});

// Register a handler that can save the current state of the machine.
// Note that this can be called from a different thread if the runAsync/saveAsync options are specifed.
machine->OnSave([](const char* json, IController* ioController)
{
  // Typically, the on save logic would be handled by your custom io controller:
  // ioController->SaveRom(json), but is left here for simplicity.

  // Write the save state to disk
  std::ofstream ofs("myProgramSave.json");
  ofs.write(json, strlen(json));  
  return errc::no_error;
});

// Register a method that will be called when there is time to do so.
// It will always be called from the same thread from which IMachine::Run was invoked.
// This method will be called continuously when possible until it returns true, at which time
// the machine will exit.
machine->OnIdle([](IController* ioController)
{
  // Typically, this method will be used to handle the processing of information
  // that is not related to the running of the engine, this may involve interaction
  // with other io devices for example, ie; handling io events.
  return ioController->ProcessIoEvent();
});

// Set the clock resolution - not setting this will run the
// machine as fast as possible (default)
machine->SetOptions(R"({"clockSamplingFreq":50})"); // 50 Hz - 20 milliseconds intervals

// Create a custom IO Controller and attach it to the machine (See unit tests for examples)
machine->AttachIOController(IControllerPtr(new CustomIOController()));

// Create a custom memory controller and attach it to the machine (See unit tests for examples)
machine->AttachMemoryController(IControllerPtr(new CustomMemoryController()));

// Run the machine sychronously with the registered on idle handler.
auto runTime = machine->Run();

// Blocks here until the custom IO controller GenerateInterrupt override generates an ISR::Quit
// interrupt or the registered IMachine::OnIdle handler returns true.

// Run the machine asychronously - this can be done by setting the following json config option
machine->SetOptions(R"({"runAsync":true})");
// The engine will run on a separate thread to the registered on idle handler
runTime = machine->Run();

// Blocks here until the custom IO controller GenerateInterrupt override generates an ISR::Quit
// interrupt or the registered IMachine::OnIdle handler returns true.

// We are done
return 0;
```

### Configuration Options

A number of configuration options are available that can be used to control the behaviour of the machine.<br>
The options must be supplied in json format defined by one of the following protocols:

| Protocol | Remarks                                                               |
|:---------|:--------------------------------------------------------------------- |
| file://  | Load json from a file on local disk specified in the remaining string |
| json://  | Load json from the remaining string                                   |

The following table describes the supported options (note, when no option is specifed the one marked as default will be used):

| Option                | Type   | Value              | Remarks                                                                             |
|:----------------------|:-------|:-------------------|:------------------------------------------------------------------------------------|
| clockSamplingFreq     | double | -1 (default)       | Run meen as fast as possible with the highest possible host clock sampling frequency|
| ^                     | ^      | 0 Hz               | Run meen at realtime with the highest possible host clock sampling frequency        |
| ^                     | ^      | 0, 1000+ Hz        | Will always spin the host cpu to maintain the clock speed and is not recommended    |
| ^                     | ^      | n Hz               | A request in Hertz as to how frequently meen's clock will tick. Note: this is only a request and while best efforts are made to honour it, the consistency of the tick rate will not be perfect, especially at higher sampling frequencies when no high resolution host clock is available|
| compressor            | string | "zlib" (default)   | Use zlib compression library to compress the ram when saving its state              |
| ^                     | ^      | "none"             | No compression will be used when saving the state of the ram                        |
| encoder               | string | "base64" (default) | The binary to text encoder to use when saving the machine state ram to json         |
| isrFreq               | double | 0 (default)        | Service interrupts at the completion of each instruction                            |
| ^                     | ^      | n                  | The number of times interrupts will be serviced per emulated cpu clock speed. For example, an i8080 running at 2Mhz with an isrFreq of 50 will service interrupts every 40000 ticks|
| loadAsync             | bool   | true               | Run the load initiation handler on a separate thread                                |
| ^                     | ^      | false (default)    | Run the load initiation handler from the thread specified by the `runAsync` option  |
| runAsync              | bool   | true               | `IMachine::Run` will launch its execution loop on a separate thread                 |
| ^                     | ^      | false (default)    | `IMachine::Run` will run its execution loop on the current thread                   |
| saveAsync             | bool   | true               | Run the save completion handler on a separate thread                                |
| ^                     | ^      | false (default)    | Run the save completion handler from the thread specifed by the `runAsync` option   |

Configuration options can be supplied to MEEN via the IMachine::SetOptions api method:

C++ - `machine->SetOptions(R"(json://{"isrFreq":1})")`<br>
Python - `self.machine.SetOptions(r"file://myConfig.json")`

See `IMachine::SetOptions` for further details.

### Acknowledgements

Special thanks to the following sites:

- [i8080 Manual](https://altairclone.com/downloads/manuals/8080%20Programmers%20Manual.pdf)
- [CPU Tests](https://altairclone.com/downloads/cpu_tests/)
- [Pretty Assmebler](https://caglrc.cc/~svo/i8080/)
- [Windows Version Resource](https://github.com/halex2005/CMakeHelpers)