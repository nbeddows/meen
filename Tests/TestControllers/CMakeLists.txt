set(lib_name TestControllers)

set(testcontroller_module_files
  ../../Base/${module_dir}/Base.ixx
  ../../Controller/${module_dir}/IController.ixx
  ../../Controller/${module_dir}/IMemoryController.ixx
  ${module_dir}/BaseIoController.ixx
  ${module_dir}/ControllerFactory.ixx
  ${module_dir}/CpmIoController.ixx
  ${module_dir}/MemoryController.ixx
  ${module_dir}/TestIoController.ixx
)

set(testcontroller_source_files
  ${source_dir}/BaseIoController.cpp
  ${source_dir}/ControllerFactory.cpp
  ${source_dir}/CpmIoController.cpp
  ${source_dir}/MemoryController.cpp
  ${source_dir}/TestIoController.cpp
)

SOURCE_GROUP("Module Files" FILES ${testcontroller_module_files})
SOURCE_GROUP("Source Files" FILES ${testcontroller_source_files})

add_library(${lib_name} STATIC
  ${testcontroller_module_files}
  ${testcontroller_source_files}
)