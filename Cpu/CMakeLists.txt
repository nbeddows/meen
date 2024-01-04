set (lib_name Cpu)

set (${lib_name}_module_files
	${module_dir}/I${lib_name}.ixx
	${module_dir}/8080.ixx
	${module_dir}/${lib_name}Factory.ixx
)

set (${lib_name}_source_files
	${source_dir}/8080.cpp
	${source_dir}/${lib_name}Factory.cpp
)

add_target(${lib_name} STATIC "" "${${lib_name}_module_files}" "${${lib_name}_source_files}")

if(CMAKE_COMPILER_IS_GNUCXX)
	target_compile_options(${lib_name} PRIVATE -fPIC -Wno-attributes)
endif()

target_include_directories(${lib_name} PRIVATE ${CMAKE_SOURCE_DIR}/Base/${include_dir})

add_dependencies(${lib_name} CpuClock SystemBus)