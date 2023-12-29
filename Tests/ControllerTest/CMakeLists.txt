set(exe_name ControllerTest)

set(${exe_name}_source_files
  ${source_dir}/ControllerTest.cpp
)

add_target(${exe_name} EXE "" "${${exe_name}_source_files}")

target_link_libraries(${exe_name} PRIVATE
  GTest::gtest
  GTest::gtest_main
  TestControllers
)

target_compile_definitions(${exe_name} PRIVATE PROGRAMS_DIR=\"${CMAKE_SOURCE_DIR}/Tests/Programs/\")