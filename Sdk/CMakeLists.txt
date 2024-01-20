set(major 1)
set(minor 3)
set(bugfix 0)
set(target_name Sdk)
set(sdkDir ${CMAKE_BINARY_DIR}/${target_name}/mach-emu)

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
file(WRITE ${CMAKE_BINARY_DIR}/${target_name}/RootCMakeLists.txt
  "cmake_minimum_required (VERSION 3.25)\n"
  "#set(CMAKE_CXX_COMPILER g++-13)\n"
  "set(CMAKE_CXX_STANDARD 20)\n"
  "set(CMAKE_CXX_STANDARD_REQUIRED ON)\n"
  "set(CMAKE_CXX_EXTENSIONS OFF)\n"
  "set(CMAKE_PREFIX_PATH \${CMAKE_SOURCE_DIR}/bin/)\n"
  "set(include_dir include)\n"
  "set(module_dir module)\n"
  "set(source_dir source)\n"
  "project(mach-emu)\n"
  "find_library(libMachEmu MachEmu)\n"
  "if(CMAKE_COMPILER_IS_GNUCXX)\n"
  "\tset(STD_MODULES cstdint memory string vector)\n"
  "\tinclude(\${CMAKE_SOURCE_DIR}/CMakeIncludes/stdModules.cmake)\n"
  "endif()\n"
  "include(\${CMAKE_SOURCE_DIR}/CMakeIncludes/addTarget.cmake)\n"
  "add_subdirectory(Tests)\n"
  "install(FILES \${CMAKE_SOURCE_DIR}/bin/${prefix}MachEmu.${postfix} DESTINATION lib)\n"
  "install(DIRECTORY \${CMAKE_SOURCE_DIR}/Base/include/Base DESTINATION include)\n"
  "install(DIRECTORY \${CMAKE_SOURCE_DIR}/Controller/include/Controller DESTINATION include)\n"
  "install(DIRECTORY \${CMAKE_SOURCE_DIR}/Machine/include/Machine DESTINATION include)\n"
  "if(MSVC)\n"
  "\tinstall(FILES \${CMAKE_SOURCE_DIR}/bin/MachEmu.lib DESTINATION lib)\n"
  "endif()\n"
)

# copy out the required modules/sources/libraries, perform static analysis
add_custom_target(
  ${target_name}
  COMMAND cppcheck -i build -i Tests --inline-suppr --enable=warning,style,performance,portability,unusedFunction --std=c++20 "${CMAKE_SOURCE_DIR}"
  COMMAND ${CMAKE_COMMAND} -E make_directory ${sdkDir}
  COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different ${CMAKE_SOURCE_DIR}/Base/include ${sdkDir}/Base/include
  COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different ${CMAKE_SOURCE_DIR}/CMakeIncludes ${sdkDir}/CMakeIncludes/
  COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different ${CMAKE_SOURCE_DIR}/Controller/include ${sdkDir}/Controller/include
  COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different ${CMAKE_SOURCE_DIR}/Machine/include ${sdkDir}/Machine/include
  COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different ${CMAKE_SOURCE_DIR}/Tests ${sdkDir}/Tests/
  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_BINARY_DIR}/Sdk/RootCMakeLists.txt ${sdkDir}/CMakeLists.txt
  ${copyLib}
  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/${prefix}MachEmu.${postfix} ${sdkDir}/bin/${prefix}MachEmu.${postfix}
  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/CHANGELOG.md ${sdkDir}/CHANGELOG.md
  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/LICENSE.md ${sdkDir}/LICENSE.md
  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/README.md ${sdkDir}/README.md
  COMMAND ${CMAKE_COMMAND} -E chdir ${sdkDir} doxygen ${CMAKE_SOURCE_DIR}/Docs/Doxyfile
  COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different ${CMAKE_SOURCE_DIR}/Docs/images ${sdkDir}/Docs/html/Docs/images
  COMMAND ${CMAKE_COMMAND} -E tar -czf ${sdkDir}-v${major}.${minor}.${bugfix}-${CMAKE_SYSTEM}-${CMAKE_SYSTEM_PROCESSOR}-dev.tar.gz ${sdkDir}
  COMMAND ${CMAKE_COMMAND} -E rm -rf ${sdkDir}
)

add_dependencies(${target_name} MachEmu)
