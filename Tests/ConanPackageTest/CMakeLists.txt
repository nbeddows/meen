cmake_minimum_required(VERSION 3.25)
project(MachEmuPackageTest CXX)

if(MSVC)
  set(buildType $<CONFIG>)
else()
  set(buildType ${CMAKE_BUILD_TYPE})
endif()

find_package(mach_emu CONFIG REQUIRED)

add_executable(MachEmuPackageTest source/MachEmuPackageTest.cpp)

if(DEFINED artifactsDir OR DEFINED zlibDllDir)
    include(${CMAKE_SOURCE_DIR}/../MachineTest/pythonTestDeps.cmake)

    add_custom_command(
        TARGET MachEmuPackageTest POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_BINARY_DIR}/MachineTestDeps${buildType}.py ${CMAKE_CURRENT_SOURCE_DIR}/source/MachineTestDeps.py
    )
endif()

target_link_libraries(MachEmuPackageTest mach_emu::mach_emu)
