set (lib_name Controller)

set (controller_module_files
	${module_dir}/${lib_name}.ixx
	${module_dir}/I${lib_name}.ixx
)

set (controller_source_files
	${source_dir}/${lib_name}.cpp
)

SOURCE_GROUP("Module Files" FILES ${controller_module_files})
SOURCE_GROUP("Source Files" FILES ${controller_source_files})

add_library(${lib_name}
	${controller_module_files}
	${controller_source_files}
)

add_dependencies(${lib_name} Base SystemBus)