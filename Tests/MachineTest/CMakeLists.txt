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

set(exe_name MachineTest)

set(${exe_name}_source_files
  ${source_dir}/MachineTest.cpp
)

SOURCE_GROUP("Source Files" FILES ${${exe_name}_source_files})

add_executable(${exe_name} ${${exe_name}_source_files})

if(enablePythonModule STREQUAL ON)
    include(pythonTestDeps.cmake)

    add_custom_command(
        TARGET ${exe_name} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_BINARY_DIR}/MachineTestDeps${buildType}.py ${CMAKE_CURRENT_SOURCE_DIR}/source/MachineTestDeps.py
        )
endif()

target_link_libraries(${exe_name} PRIVATE
  GTest::GTest
  ${libMachEmu}
  nlohmann_json::nlohmann_json
  TestControllers
)

if(${CMAKE_CROSSCOMPILING})
  target_compile_definitions(${exe_name} PRIVATE PROGRAMS_DIR=\"Programs/\")
else()
  target_compile_definitions(${exe_name} PRIVATE PROGRAMS_DIR=\"${CMAKE_SOURCE_DIR}/Tests/Programs/\")
endif()

target_include_directories(${exe_name} PRIVATE ${CMAKE_SOURCE_DIR}/Base/${include_dir})
target_include_directories(${exe_name} PRIVATE ${CMAKE_SOURCE_DIR}/Controller/${include_dir})
target_include_directories(${exe_name} PRIVATE ${CMAKE_SOURCE_DIR}/Machine/${include_dir})
target_include_directories(${exe_name} PRIVATE ${CMAKE_SOURCE_DIR}/Tests/TestControllers/${include_dir})