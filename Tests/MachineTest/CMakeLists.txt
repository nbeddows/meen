set(exe_name MachineTest)

set(${exe_name}_source_files
  ${source_dir}/8080Test.cpp
  ${source_dir}/MachineTest.cpp
)

add_target(${exe_name} EXE "" "" "${${exe_name}_source_files}")

target_link_libraries(${exe_name} PRIVATE
  GTest::gtest
  GTest::gtest_main
  ${libMachEmu}
  TestControllers
)

target_compile_definitions(${exe_name} PRIVATE PROGRAMS_DIR=\"${CMAKE_SOURCE_DIR}/Tests/Programs/\")
target_include_directories(${exe_name} PRIVATE ${CMAKE_SOURCE_DIR}/Base/${include_dir})
target_include_directories(${exe_name} PRIVATE ${CMAKE_SOURCE_DIR}/Controller/${include_dir})
target_include_directories(${exe_name} PRIVATE ${CMAKE_SOURCE_DIR}/Machine/${include_dir})
