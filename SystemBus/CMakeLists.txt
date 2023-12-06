set (lib_name SystemBus)

set (${lib_name}_module_files
	${module_dir}/${lib_name}.ixx
)

addTarget(${lib_name} ${${lib_name}_module_files})