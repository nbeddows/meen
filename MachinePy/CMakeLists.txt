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

set (lib_name ${libMachEmu}Py)

set (${lib_name}_include_files
	${include_dir}/MachinePy/ControllerPy.h
	${include_dir}/MachinePy/MachineHolder.h
)

set (${lib_name}_source_files
	${source_dir}/ControllerPy.cpp
	${source_dir}/MachineHolder.cpp
	${source_dir}/MachineModule.cpp
)

SOURCE_GROUP("Include Files" FILES ${${lib_name}_include_files})
SOURCE_GROUP("Source Files" FILES ${${lib_name}_source_files})

pybind11_add_module(${lib_name} ${${lib_name}_include_files} ${${lib_name}_source_files})

target_include_directories(${lib_name} PRIVATE ${CMAKE_SOURCE_DIR}/Base/${include_dir})
target_include_directories(${lib_name} PRIVATE ${CMAKE_SOURCE_DIR}/Controller/${include_dir})
target_include_directories(${lib_name} PRIVATE ${CMAKE_SOURCE_DIR}/Machine/${include_dir})
target_include_directories(${lib_name} PRIVATE ${CMAKE_SOURCE_DIR}/MachinePy/${include_dir})

target_link_directories(${lib_name} PRIVATE ${Python_LIBRARY_DIRS})
target_link_libraries(${lib_name} PRIVATE pybind11::headers ${libMachEmu})

install(TARGETS ${lib_name} DESTINATION ${runtimeDir})