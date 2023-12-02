include(FetchContent)
FetchContent_Declare(
  googletest
  # Specify the commit you depend on and update it regularly.
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

set(exe_name Tests)

set(test_module_files
  ../Base/${module_dir}/Base.ixx
  ../Controller/${module_dir}/IController.ixx
  ../Machine/${module_dir}/MachineFactory.ixx
  ../Machine/${module_dir}/IMachine.ixx
)

set(test_source_files
  MachineTest/${source_dir}/MachineTest.cpp
)

SOURCE_GROUP("Module Files" FILES test_module_files)
SOURCE_GROUP("Source Files" FILES test_source_files)

add_executable(${exe_name}
  ${test_module_files}
  ${test_source_files}
)

target_link_libraries(${exe_name} PRIVATE
  gtest
  gtest_main
  MachEmu
)