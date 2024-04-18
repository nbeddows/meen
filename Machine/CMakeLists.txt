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

set (lib_name MachEmu)

set (${lib_name}_include_files
	${include_dir}/Machine/Machine.h
	${include_dir}/Machine/IMachine.h
	${include_dir}/Machine/MachineFactory.h
)

if(MSVC)
	set (${lib_name}_resource_files
		${resource_dir}/Machine.rc
	)
endif()

set (${lib_name}_source_files
	${source_dir}/Machine.cpp
	${source_dir}/MachineFactory.cpp
)

SOURCE_GROUP("Include Files" FILES ${${lib_name}_include_files})
SOURCE_GROUP("Source Files" FILES ${${lib_name}_source_files})
if(MSVC)
	SOURCE_GROUP("Resource Files" FILES ${${lib_name}_source_files})
endif()

add_library(${lib_name} SHARED ${${lib_name}_include_files} ${${lib_name}_resource_files} ${${lib_name}_source_files})

if(CMAKE_COMPILER_IS_GNUCXX)
	target_compile_options(${lib_name} PRIVATE -fvisibility=hidden)
endif()

target_compile_definitions(${lib_name} PRIVATE ${lib_name}_VERSION=\"${machEmuVersion}\")
target_include_directories(${lib_name} PRIVATE ${CMAKE_SOURCE_DIR}/Base/${include_dir})
target_include_directories(${lib_name} PRIVATE ${CMAKE_SOURCE_DIR}/Cpu/${include_dir})
target_include_directories(${lib_name} PRIVATE ${CMAKE_SOURCE_DIR}/CpuClock/${include_dir})
target_include_directories(${lib_name} PRIVATE ${CMAKE_SOURCE_DIR}/Controller/${include_dir})
target_include_directories(${lib_name} PRIVATE ${CMAKE_SOURCE_DIR}/Machine/${include_dir})
target_include_directories(${lib_name} PRIVATE ${CMAKE_SOURCE_DIR}/Opt/${include_dir})
target_include_directories(${lib_name} PRIVATE ${CMAKE_SOURCE_DIR}/SystemBus/${include_dir})
target_include_directories(${lib_name} PRIVATE ${CMAKE_SOURCE_DIR}/Utils/${include_dir})

add_dependencies(${lib_name} Cpu CpuClock Opt Utils)

set_target_properties(${lib_name} PROPERTIES VERSION ${machEmuVersion} SOVERSION ${machEmuVersion})

target_link_libraries(${lib_name} PRIVATE
	Cpu
	CpuClock
	Opt
	nlohmann_json::nlohmann_json
	Utils
)