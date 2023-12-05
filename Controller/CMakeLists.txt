set (lib_name Controller)

set (controller_module_files
	${module_dir}/I${lib_name}.ixx
	${module_dir}/IMemory${lib_name}.ixx
)

SOURCE_GROUP("Module Files" FILES ${controller_module_files})

add_library(${lib_name}
	${controller_module_files}
)

add_dependencies(${lib_name} Base)