set (lib_name Controller)

set (${lib_name}_module_files
	${module_dir}/I${lib_name}.ixx
	${module_dir}/IMemory${lib_name}.ixx
)

add_target(${lib_name} "${${lib_name}_module_files}" "")

add_dependencies(${lib_name} Base)

