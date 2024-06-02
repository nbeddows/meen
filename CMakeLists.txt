# Copyright (c) 2021-2024 Nicolas Beddows <nicolas.beddows@gmail.com>

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

cmake_minimum_required (VERSION 3.25)
set(include_dir include)
set(resource_dir resource)
set(source_dir source)
set(libMachEmu mach_emu)
set(major 1)
set(minor 5)
set(bugfix 1)

project(${libMachEmu} VERSION ${major}.${minor}.${bugfix})

if(MSVC)
  set(buildType $<CONFIG>)
else()
  set(buildType ${CMAKE_BUILD_TYPE})
endif()

set(artifactsDir ${CMAKE_SOURCE_DIR}/artifacts/${buildType}/${CMAKE_SYSTEM_PROCESSOR})

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${artifactsDir}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${artifactsDir}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${artifactsDir}/bin)

option(enableZlib "Enable zlib compression" ON)
option(enablePythonModule "Enable Python module via Pybind" OFF)

if(enablePythonModule)
    find_package(Python REQUIRED COMPONENTS Interpreter Development.Module)
    find_package(pybind11 REQUIRED)
endif()

if(enableZlib)
    find_package(ZLIB REQUIRED)
endif()

find_package(base64 REQUIRED)
find_package(hash-library REQUIRED)
find_package(gtest REQUIRED)
find_package(nlohmann_json REQUIRED)

set(machEmuVersion ${CMAKE_PROJECT_VERSION})

add_subdirectory(Base)
add_subdirectory(Controller)
add_subdirectory(Cpu)
add_subdirectory(CpuClock)
add_subdirectory(Machine)
if(enablePythonModule)
    add_subdirectory(MachinePy)
endif()
add_subdirectory(Opt)
add_subdirectory(Sdk)
add_subdirectory(SystemBus)
add_subdirectory(Tests)
add_subdirectory(Utils)
