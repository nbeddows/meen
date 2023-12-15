set(exe_name ControllerTest)

set(${exe_name}_source_files
  ${source_dir}/ControllerTest.cpp
)

add_target(${exe_name} EXE "" "${${exe_name}_source_files}")

add_dependencies(${exe_name} TestControllers)

target_link_libraries(${exe_name} PRIVATE
  GTest::gtest
  GTest::gtest_main
  TestControllers
)

add_custom_command(
  TARGET ${exe_name} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  ${CMAKE_SOURCE_DIR}/Tests/Programs
  ${CMAKE_BINARY_DIR}/Programs)

#set_target_properties(${exe_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Bin)