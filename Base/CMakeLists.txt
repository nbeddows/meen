set (lib_name Base)

set (${lib_name}_module_files
	${module_dir}/${lib_name}.ixx
)

add_target(${lib_name} ${${lib_name}_module_files} "")