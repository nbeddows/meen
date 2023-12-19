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

set_target_properties(${exe_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)