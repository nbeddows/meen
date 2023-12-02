set (lib_name MachEmu)

set (machine_module_files
	${module_dir}/IMachine.ixx
	${module_dir}/Machine.ixx
	${module_dir}/MachineFactory.ixx
)

set (machine_source_files
	${source_dir}/Machine.cpp
	${source_dir}/MachineFactory.cpp
)

SOURCE_GROUP("Module Files" FILES ${machine_module_files})
SOURCE_GROUP("Source Files" FILES ${machine_source_files})

add_library(${lib_name} SHARED
	${machine_module_files}
	${machine_source_files}
)

add_dependencies(${lib_name} Base Controller Cpu CpuClock SystemBus)

target_link_libraries(${lib_name} PRIVATE
	Controller
	Cpu
	CpuClock
)