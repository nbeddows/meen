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

set(lib_name TestControllers)

set(${lib_name}_include_files
  ${include_dir}/${lib_name}/BaseIoController.h
  ${include_dir}/${lib_name}/CpmIoController.h
  ${include_dir}/${lib_name}/MemoryController.h
  ${include_dir}/${lib_name}/TestIoController.h
)

set(${lib_name}_source_files
  ${source_dir}/BaseIoController.cpp
  ${source_dir}/CpmIoController.cpp
  ${source_dir}/MemoryController.cpp
  ${source_dir}/TestIoController.cpp
)

SOURCE_GROUP("Include Files" FILES ${${lib_name}_include_files})
SOURCE_GROUP("Source Files" FILES ${${lib_name}_source_files})

add_library(${lib_name} STATIC ${${lib_name}_include_files} ${${lib_name}_source_files})

if(CMAKE_COMPILER_IS_GNUCXX)
	target_compile_options(${lib_name} PRIVATE -Wno-attributes)
endif()

target_include_directories(${lib_name} PRIVATE ${CMAKE_SOURCE_DIR}/Base/${include_dir})
target_include_directories(${lib_name} PRIVATE ${CMAKE_SOURCE_DIR}/Controller/${include_dir})
target_include_directories(${lib_name} PRIVATE ${CMAKE_SOURCE_DIR}/Tests/TestControllers/${include_dir})