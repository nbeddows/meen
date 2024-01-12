set (lib_name MachEmu)

set (${lib_name}_include_files
	${include_dir}/Machine/IMachine.h
	${include_dir}/Machine/MachineFactory.h
)

set (${lib_name}_module_files
	${module_dir}/Machine.ixx
)

set (${lib_name}_source_files
	${source_dir}/Machine.cpp
	${source_dir}/MachineFactory.cpp
)

add_target(${lib_name} SHARED "${${lib_name}_include_files}" "${${lib_name}_module_files}" "${${lib_name}_source_files}")

if(CMAKE_COMPILER_IS_GNUCXX)
	target_compile_options(${lib_name} PRIVATE -fvisibility=hidden)
endif()

target_include_directories(${lib_name} PRIVATE ${CMAKE_SOURCE_DIR}/Base/${include_dir})
target_include_directories(${lib_name} PRIVATE ${CMAKE_SOURCE_DIR}/Controller/${include_dir})
target_include_directories(${lib_name} PRIVATE ${CMAKE_SOURCE_DIR}/Machine/${include_dir})

add_dependencies(${lib_name} Cpu CpuClock SystemBus)

target_link_libraries(${lib_name} PRIVATE
	Cpu
	CpuClock
)