set (lib_name SystemBus)

set (system_bus_module_files
	${module_dir}/${lib_name}.ixx
)

SOURCE_GROUP("Module Files" FILES ${system_bus_module_files})

add_library(${lib_name}
	${system_bus_module_files}
)