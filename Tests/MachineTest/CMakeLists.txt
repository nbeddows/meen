set(exe_name MachineTest)

set(${exe_name}_module_files
  ${CMAKE_SOURCE_DIR}/Machine/${module_dir}/IMachine.ixx
  ${CMAKE_SOURCE_DIR}/Machine/${module_dir}/MachineFactory.ixx
)

set(${exe_name}_source_files
  ${source_dir}/MachineTest.cpp
)

add_target(${exe_name} EXE
  "${${exe_name}_module_files}"
  "${${exe_name}_source_files}"
)

add_dependencies(${exe_name} TestControllers)

target_link_libraries(${exe_name} PRIVATE
  GTest::gtest
  GTest::gtest_main
  MachEmu
  TestControllers
)

add_custom_command(
  TARGET ${exe_name} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  ${CMAKE_SOURCE_DIR}/Tests/Programs
  ${CMAKE_BINARY_DIR}/Programs)

set_target_properties(${exe_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Bin)