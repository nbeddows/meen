set(exe_name ControllerTest)

set(test_source_files
  ${source_dir}/ControllerTest.cpp
)

SOURCE_GROUP("Source Files" FILES ${test_source_files})

add_executable(${exe_name}
  ${test_source_files}
)

add_dependencies(${exe_name} TestControllers)

target_link_libraries(${exe_name} PRIVATE
  gtest
  gtest_main
  TestControllers
)

add_custom_command(
  TARGET ${exe_name} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  ${CMAKE_SOURCE_DIR}/Tests/Programs
  ${CMAKE_BINARY_DIR}/Programs)

set_target_properties(${exe_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Bin)