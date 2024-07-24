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
set(minor 6)
set(bugfix 2)

project(${libMachEmu} VERSION ${major}.${minor}.${bugfix})

if(MSVC)
  set(buildType $<CONFIG>)
else()
  set(buildType ${CMAKE_BUILD_TYPE})
endif()

if(NOT DEFINED buildArch)
  set(buildArch ${CMAKE_SYSTEM_PROCESSOR})
endif()

if(NOT DEFINED archiveDir)
  set(archiveDir "lib")
endif()

if(NOT DEFINED runtimeDir)
  set(runtimeDir "bin")
endif()

set(artifactsDir $<1:${CMAKE_SOURCE_DIR}/artifacts/${buildType}/${buildArch}>)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${artifactsDir}/${archiveDir})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${artifactsDir}/${runtimeDir})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${artifactsDir}/${runtimeDir})

if(enablePythonModule STREQUAL ON)
    find_package(Python REQUIRED COMPONENTS Interpreter Development.Module)
    find_package(pybind11 REQUIRED)
endif()

if(enableZlib STREQUAL ON)
    find_package(ZLIB REQUIRED)
endif()

if (NOT BUILD_TESTING STREQUAL OFF)
  find_package(GTest REQUIRED)
endif()

find_package(base64 REQUIRED)
find_package(hash-library REQUIRED)
find_package(nlohmann_json REQUIRED)

set(machEmuVersion ${CMAKE_PROJECT_VERSION})

add_subdirectory(Base)
add_subdirectory(Controller)
add_subdirectory(Cpu)
add_subdirectory(CpuClock)
add_subdirectory(Machine)
if(enablePythonModule STREQUAL ON)
    add_subdirectory(MachinePy)
endif()
add_subdirectory(Opt)
add_subdirectory(Sdk)
add_subdirectory(SystemBus)
if (NOT BUILD_TESTING STREQUAL OFF)
  add_subdirectory(Tests)
endif()
add_subdirectory(Utils)

install(FILES LICENSE.md DESTINATION licenses RENAME LICENSE)
