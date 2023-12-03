set (lib_name Base)

set (base_module_files
	${module_dir}/${lib_name}.ixx
)

SOURCE_GROUP("Module Files" FILES ${base_module_files})

add_library(${lib_name}
	${base_module_files}
)