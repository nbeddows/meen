set(lib_name TestControllers)

set(${lib_name}_module_files
  ${module_dir}/BaseIoController.ixx
  ${module_dir}/CpmIoController.ixx
  ${module_dir}/MemoryController.ixx
  ${module_dir}/TestIoController.ixx
)

set(${lib_name}_source_files
  ${source_dir}/BaseIoController.cpp
  ${source_dir}/CpmIoController.cpp
  ${source_dir}/MemoryController.cpp
  ${source_dir}/TestIoController.cpp
)

add_target(${lib_name}
  STATIC
  "${${lib_name}_module_files}"
  "${${lib_name}_source_files}"
)

add_dependencies(${lib_name} Controller)

if(CMAKE_COMPILER_IS_GNUCXX)
	target_compile_options(${lib_name} PRIVATE -Wno-attributes)
endif()