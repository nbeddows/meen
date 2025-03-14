cmake_minimum_required(VERSION 3.25)
project(MeenPackageTest CXX)
set(CMAKE_CXX_STANDARD 23)

if(NOT BUILD_SHARED_LIBS STREQUAL ON)
  if(DEFINED WIN32)
    set(static_lib meen_STATIC)
  endif()
endif()

find_package(meen CONFIG REQUIRED)

add_executable(MeenPackageTest source/MeenPackageTest.cpp)

if(DEFINED artifacts_dir OR DEFINED zlib_bin_dir)
  if(MSVC)
    set(build_type $<CONFIG>)
  else()
    set(build_type ${CMAKE_BUILD_TYPE})
  endif()
    
  include(../pythonTestDeps.cmake)
  generatePythonUnitTestDeps(${artifacts_dir} testControllersPathNotUsed testProgramsPathNoUsed ${zlib_bin_dir})

  add_custom_command(
    TARGET MeenPackageTest POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_BINARY_DIR}/MachineTestDeps${build_type}.py ${CMAKE_CURRENT_SOURCE_DIR}/source/MachineTestDeps.py
  )
endif()

target_compile_definitions(MeenPackageTest PRIVATE ${static_lib})
target_link_libraries(MeenPackageTest meen::meen)
