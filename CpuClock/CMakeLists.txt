set (lib_name CpuClock)

set (cpuclock_module_files
	${module_dir}/${lib_name}.ixx
	${module_dir}/I${lib_name}.ixx
)

set (cpuclock_source_files
	${source_dir}/${lib_name}.cpp
	${source_dir}/${lib_name}Factory.cpp
)

SOURCE_GROUP("Module Files" FILES ${cpuclock_module_files})
SOURCE_GROUP("Source Files" FILES ${cpuclock_source_files})

add_library(${lib_name}
	${cpuclock_module_files}
	${cpuclock_source_files}
)

add_dependencies(${lib_name} SystemBus)