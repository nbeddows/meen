set (lib_name Base)

set (${lib_name}_include_files
	${include_dir}/${lib_name}/${lib_name}.h
)

add_target(${lib_name} INTERFACE "${${lib_name}_include_files}" "" "")