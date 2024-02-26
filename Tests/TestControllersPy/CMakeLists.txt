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

set (lib_name TestControllersPy)

set (${lib_name}_source_files
	${source_dir}/${lib_name}.cpp
)

SOURCE_GROUP("Source Files" FILES ${${lib_name}_source_files})

find_package(Python3 REQUIRED COMPONENTS Interpreter Development.Module)
find_package(pybind11 REQUIRED)

pybind11_add_module(${lib_name} ${${lib_name}_source_files})

if(CMAKE_COMPILER_IS_GNUCXX)
	target_compile_options(${lib_name} PRIVATE -fPIC -Wno-attributes -x c++ -fmodules-ts)

	set(cachePath ${CMAKE_BINARY_DIR}/gcm.cache)

	add_custom_target(
        ${lib_name}ModuleSymlink
        COMMAND ${CMAKE_COMMAND} -E create_symlink
        ${cachePath}
        gcm.cache
    )

    add_dependencies(${lib_name} ${lib_name}ModuleSymlink)
	
endif()

target_include_directories(${lib_name} PRIVATE ${CMAKE_SOURCE_DIR}/Base/${include_dir})
target_include_directories(${lib_name} PRIVATE ${CMAKE_SOURCE_DIR}/Controller/${include_dir})
# target_include_directories(${lib_name} PRIVATE ${CMAKE_SOURCE_DIR}/Test/TestControllers/${include_dir})

target_link_directories(${lib_name} PRIVATE ${Python_LIBRARY_DIRS})
target_link_libraries(${lib_name} PRIVATE pybind11::headers MachEmu TestControllers)