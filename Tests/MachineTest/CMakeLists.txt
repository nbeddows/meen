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

if (MSVC)
  if(enableZlib)
    string(REPLACE "include" "bin" ZLIB_DLL_DIR ${ZLIB_INCLUDE_DIR})
    set(addZlibDllDir "import os\nos.add_dll_directory(\"${ZLIB_DLL_DIR}\")\n")
  endif()
endif()

# command to prepend dll search path to machine test script
file(WRITE "source/MachineTestDeps.py"
  "# Copyright (c) 2021-2024 Nicolas Beddows <nicolas.beddows@gmail.com>\n\n"
  "# Permission is hereby granted, free of charge, to any person obtaining a copy\n"
  "# of this software and associated documentation files (the \"Software\"), to deal\n"
  "# in the Software without restriction, including without limitation the rights\n"
  "# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell\n"
  "# copies of the Software, and to permit persons to whom the Software is\n"
  "# furnished to do so, subject to the following conditions:\n\n"
  "# The above copyright notice and this permission notice shall be included in all\n"
  "# copies or substantial portions of the Software.\n\n"
  "# THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR\n"
  "# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\n"
  "# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE\n"
  "# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER\n"
  "# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,\n"
  "# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE\n"
  "# SOFTWARE.\n\n"
  ${addZlibDllDir}
  "import sys\n"
  "# absolute path to Python controller test modules\n"
  "sys.path.append(\"${CMAKE_SOURCE_DIR}/Tests/TestControllers/source\")\n"
  "# absolute path to the C++ controller test module\n"
  "sys.path.append(\"${CMAKE_SOURCE_DIR}/bin\")\n"
)

target_link_libraries(${exe_name} PRIVATE
  GTest::gtest
  GTest::gtest_main
  ${libMachEmu}
  nlohmann_json::nlohmann_json
  TestControllers
)

target_compile_definitions(${exe_name} PRIVATE PROGRAMS_DIR=\"${CMAKE_SOURCE_DIR}/Tests/Programs/\")
target_include_directories(${exe_name} PRIVATE ${CMAKE_SOURCE_DIR}/Base/${include_dir})
target_include_directories(${exe_name} PRIVATE ${CMAKE_SOURCE_DIR}/Controller/${include_dir})
target_include_directories(${exe_name} PRIVATE ${CMAKE_SOURCE_DIR}/Machine/${include_dir})
target_include_directories(${exe_name} PRIVATE ${CMAKE_SOURCE_DIR}/Tests/TestControllers/${include_dir})