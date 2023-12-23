set (lib_name MachEmu)

set (${lib_name}_module_files
	${module_dir}/IMachine.ixx
	${module_dir}/Machine.ixx
	${module_dir}/MachineFactory.ixx
)

set (${lib_name}_source_files
	${source_dir}/Machine.cpp
	${source_dir}/MachineFactory.cpp
)

add_target(${lib_name} SHARED
	"${${lib_name}_module_files}"
	"${${lib_name}_source_files}"
)

if(CMAKE_COMPILER_IS_GNUCXX)
	target_compile_options(${lib_name} PRIVATE -fvisibility=hidden)
endif()

add_dependencies(${lib_name} Base Controller Cpu CpuClock SystemBus)

target_link_libraries(${lib_name} PRIVATE
	Cpu
	CpuClock
)