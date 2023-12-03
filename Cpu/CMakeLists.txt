set (lib_name Cpu)

set (cpu_module_files
	${module_dir}/8080.ixx
	${module_dir}/I${lib_name}.ixx
)

set (cpu_source_files
	${source_dir}/8080.cpp
	${source_dir}/${lib_name}Factory.cpp
)

SOURCE_GROUP("Module Files" FILES ${cpu_module_files})
SOURCE_GROUP("Source Files" FILES ${cpu_source_files})

add_library(${lib_name}
	${cpu_module_files}
	${cpu_source_files}
)

add_dependencies(${lib_name} Base CpuClock SystemBus)