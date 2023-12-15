set (lib_name SystemBus)

set (${lib_name}_module_files
	${module_dir}/${lib_name}.ixx
)

add_target(${lib_name} STATIC ${${lib_name}_module_files} "")