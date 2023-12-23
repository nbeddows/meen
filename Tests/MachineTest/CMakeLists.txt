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

target_compile_definitions(${exe_name} PRIVATE PROGRAMS_DIR=\"${CMAKE_SOURCE_DIR}/Tests/Programs/\")
# Leave a new line here on purpose as the sdk appends a target_link_directories command
