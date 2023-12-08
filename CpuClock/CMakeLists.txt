set (lib_name CpuClock)

set (${lib_name}_module_files
	${module_dir}/I${lib_name}.ixx
	${module_dir}/${lib_name}.ixx
	${module_dir}/${lib_name}Factory.ixx
)

set (${lib_name}_source_files
	${source_dir}/${lib_name}.cpp
	${source_dir}/${lib_name}Factory.cpp
)


add_target(${lib_name}
	"${${lib_name}_module_files}"
	"${${lib_name}_source_files}"
)

add_dependencies(${lib_name} SystemBus)