set(major 1)
set(minor 3)
set(bugfix 0)
set(sdkDir ${CMAKE_BINARY_DIR}/Sdk/mach-emu)

if(CMAKE_COMPILER_IS_GNUCXX)
  set(escape \\)
  set(prefix lib)
  set(postfix so)
else()
  set(postfix dll)
endif()

if(MSVC)
  set(copyLib COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/MachEmu.lib ${sdkDir}/bin/MachEmu.lib)
endif()

# write the top level sdk cmake make list
file(WRITE ${CMAKE_BINARY_DIR}/Sdk/RootCMakeLists.txt
  "cmake_minimum_required (VERSION 3.25)\n"
  "#set(CMAKE_CXX_COMPILER g++-13)\n"
  "set(CMAKE_CXX_STANDARD 20)\n"
  "set(CMAKE_CXX_STANDARD_REQUIRED ON)\n"
  "set(CMAKE_CXX_EXTENSIONS OFF)\n"
  "set(CMAKE_PREFIX_PATH \${CMAKE_SOURCE_DIR}/bin/)\n"
  "set(module_dir module)\n"
  "set(source_dir source)\n"
  "project(mach-emu-sdk)\n"
  "find_library(libMachEmu MachEmu)\n"
  "if(CMAKE_COMPILER_IS_GNUCXX)\n"
  "\tset(STD_MODULES cstdint memory string vector)\n"
  "\tinclude(\${CMAKE_SOURCE_DIR}/CMakeIncludes/stdModules.cmake)\n"
  "endif()\n"
  "include(\${CMAKE_SOURCE_DIR}/CMakeIncludes/addTarget.cmake)\n"
  "add_subdirectory(Base)\n"
  "add_subdirectory(Controller)\n"
  "add_subdirectory(Machine)\n"
  "add_subdirectory(Tests)\n"
  "install(FILES \${CMAKE_SOURCE_DIR}/bin/${prefix}MachEmu.${postfix} DESTINATION lib)\n"
  "if(MSVC)\n"
  "\tinstall(FILES \${CMAKE_SOURCE_DIR}/bin/MachEmu.lib DESTINATION lib)\n"
  "endif()\n"
)

# write the machine cmake list
file(WRITE ${CMAKE_BINARY_DIR}/Sdk/MachineCMakeLists.txt
  "set (lib_name Machine)\n"
  "set (\${lib_name}_module_files \"\${module_dir}/IMachine.ixx\" \"\${module_dir}/MachineFactory.ixx\")\n"
  "add_target(\${lib_name} STATIC \"\$\{\${lib_name}_module_files}\" \"\")\n"
  "add_dependencies(\${lib_name} Controller)\n"
)

# copy out the required modules/sources/libraries, perform static analysis
add_custom_target(
  Sdk
  COMMAND cppcheck -i build -i Tests --inline-suppr --enable=warning,style,performance,portability,unusedFunction --std=c++20 "${CMAKE_SOURCE_DIR}"
  COMMAND ${CMAKE_COMMAND} -E make_directory ${sdkDir}
  COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different ${CMAKE_SOURCE_DIR}/Base ${sdkDir}/Base/
  COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different ${CMAKE_SOURCE_DIR}/CMakeIncludes ${sdkDir}/CMakeIncludes/
  COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different ${CMAKE_SOURCE_DIR}/Controller ${sdkDir}/Controller/
  COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different ${CMAKE_SOURCE_DIR}/Tests ${sdkDir}/Tests/
  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_BINARY_DIR}/Sdk/RootCMakeLists.txt ${sdkDir}/CMakeLists.txt
  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_BINARY_DIR}/Sdk/MachineCMakeLists.txt ${sdkDir}/Machine/CMakeLists.txt
  ${copyLib}
  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/${prefix}MachEmu.${postfix} ${sdkDir}/bin/${prefix}MachEmu.${postfix}
  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/Machine/module/IMachine.ixx ${sdkDir}/Machine/module/IMachine.ixx
  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/Machine/module/MachineFactory.ixx ${sdkDir}/Machine/module/MachineFactory.ixx
  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/CHANGELOG.md ${sdkDir}/CHANGELOG.md
  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/LICENSE.md ${sdkDir}/LICENSE.md
  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/README.md ${sdkDir}/README.md
  COMMAND ${CMAKE_COMMAND} -E echo "add_dependencies${escape}(\${exe_name} Machine)" >> ${sdkDir}/Tests/MachineTest/CMakeLists.txt
  COMMAND ${CMAKE_COMMAND} -E chdir ${sdkDir} doxygen ${CMAKE_SOURCE_DIR}/Docs/Doxyfile
  COMMAND ${CMAKE_COMMAND} -E tar -czf ${sdkDir}-v${major}.${minor}.${bugfix}-${CMAKE_SYSTEM}-${CMAKE_SYSTEM_PROCESSOR}-dev.tar.gz ${sdkDir}
  COMMAND ${CMAKE_COMMAND} -E rm -rf ${sdkDir}
)