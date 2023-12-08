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

if(CMAKE_COMPILER_IS_GNUCXX)
	target_compile_options(${lib_name} PRIVATE -Wno-attributes)
endif()

add_dependencies(${lib_name} SystemBus)