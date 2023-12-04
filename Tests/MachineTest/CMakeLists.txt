set(exe_name MachineTest)

set(test_module_files
  ../../Machine/${module_dir}/IMachine.ixx
  ../../Machine/${module_dir}/MachineFactory.ixx
)

set(test_source_files
  ${source_dir}/MachineTest.cpp
)

SOURCE_GROUP("Module Files" FILES ${test_module_files})
SOURCE_GROUP("Source Files" FILES ${test_source_files})

add_executable(${exe_name}
  ${test_module_files}
  ${test_source_files}
)

add_dependencies(${exe_name} TestControllers)

target_link_libraries(${exe_name} PRIVATE
  gtest
  gtest_main
  MachEmu
  TestControllers
)

add_custom_command(
  TARGET ${exe_name} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  ${CMAKE_SOURCE_DIR}/Tests/Programs
  ${CMAKE_BINARY_DIR}/Programs)

set_target_properties(${exe_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Bin)