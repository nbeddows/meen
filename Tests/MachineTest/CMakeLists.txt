set(exe_name MachineTest)

set(${exe_name}_source_files
  ${source_dir}/MachineTest.cpp
)

add_target(${exe_name} EXE
  ""
  "${${exe_name}_source_files}"
)

target_link_libraries(${exe_name} PRIVATE
  Controller
  GTest::gtest
  GTest::gtest_main
  ${libMachEmu}
  TestControllers
)

target_compile_definitions(${exe_name} PRIVATE PROGRAMS_DIR=\"${CMAKE_SOURCE_DIR}/Tests/Programs/\")
# Leave a new line here on purpose as the sdk appends an add_dependencies command
