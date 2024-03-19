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

include(FetchContent)
FetchContent_Declare(
  googletest
  # Specify the commit you depend on and update it regularly.
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

if(CMAKE_COMPILER_IS_GNUCXX)
  # our required std modules for the unit tests
  set(STD_MODULES cstdint memory string vector)

  # One time build only
  add_custom_target(TestStdModules ALL
    DEPENDS ${STD_MODULES_BUILD_TIMESTAMP_FILE}
  )

  # compile the required std modules
  add_custom_command(
    OUTPUT ${STD_MODULES_BUILD_TIMESTAMP_FILE}
    COMMAND ${CMAKE_COMMAND} -E echo "Building standard libraries modules for tests"
    COMMAND ${CMAKE_CXX_COMPILER} ${STD_MODULES_OPTIONS} ${STD_MODULES}
    COMMAND ${CMAKE_COMMAND} -E touch ${STD_MODULES_BUILD_TIMESTAMP_FILE}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/Tests
    BYPRODUCTS
  )

  # copy out the required modules
  add_custom_target(
    TestModules
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_BINARY_DIR}/Tests/TestControllers/gcm.cache/BaseIoController.gcm
    ${CMAKE_BINARY_DIR}/Tests/TestControllers/gcm.cache/CpmIoController.gcm
    ${CMAKE_BINARY_DIR}/Tests/TestControllers/gcm.cache/MemoryController.gcm
    ${CMAKE_BINARY_DIR}/Tests/TestControllers/gcm.cache/TestIoController.gcm
    ${CMAKE_BINARY_DIR}/Tests/gcm.cache
  )
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
add_subdirectory(ControllerTest)
add_subdirectory(MachineTest)
add_subdirectory(TestControllers)

if(enablePythonModule)
  add_subdirectory(TestControllersPy)
  set_target_properties(TestControllersPy PROPERTIES FOLDER "Tests")
endif()

set_target_properties(ControllerTest PROPERTIES FOLDER "Tests")
set_target_properties(MachineTest PROPERTIES FOLDER "Tests")
set_target_properties(TestControllers PROPERTIES FOLDER "Tests")

if(CMAKE_COMPILER_IS_GNUCXX)
  add_dependencies(ControllerTest TestModules)
  add_dependencies(MachineTest TestModules)
endif()
