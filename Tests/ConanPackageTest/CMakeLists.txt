cmake_minimum_required(VERSION 3.25)
project(MachEmuPackageTest CXX)

find_package(mach_emu CONFIG REQUIRED)

add_executable(MachEmuPackageTest source/MachEmuPackageTest.cpp)
target_link_libraries(MachEmuPackageTest mach_emu::mach_emu)
