# Copyright (c) 2021-2024 Nicolas Beddows <nicolas.beddows@gmail.com>

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

set(target_name Sdk)
set(sdkDir ${CMAKE_BINARY_DIR}/${target_name}/mach-emu)

if(MSVC)
  set(copyLib COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/MachEmu.lib ${sdkDir}/bin/MachEmu.lib)
  set(installLinkLib "install(FILES \${CMAKE_SOURCE_DIR}/bin/MachEmu.lib DESTINATION lib)\n")
  set(postfix dll)

  if(enablePythonModule)
	    set(pythonModule MachEmuPy.cp${Python_VERSION_MAJOR}${Python_VERSION_MINOR}-win_amd64.pyd)
	    set(testControllersModule TestControllersPy.cp${Python_VERSION_MAJOR}${Python_VERSION_MINOR}-win_amd64.pyd)
  endif()
else()
  set(escape \\)
  set(postfix so)
  set(prefix lib)
  set(versionExt .${machEmuVersion})
  set(buildModules "set(STD_MODULES array cstdint memory string vector)\ninclude(\${CMAKE_SOURCE_DIR}/CMakeIncludes/stdModules.cmake)\n")
  set(installSoLink "install(CODE \"execute_process(COMMAND \${CMAKE_COMMAND} -E create_symlink ${CMAKE_INSTALL_PREFIX}/lib/${prefix}MachEmu.${postfix}${versionExt} ${CMAKE_INSTALL_PREFIX}/lib/${prefix}MachEmu.${postfix})\")\n")
  set(soLink COMMAND ${CMAKE_COMMAND} -E create_symlink ${sdkDir}/bin/${prefix}MachEmu.${postfix}${versionExt} ${sdkDir}/bin/${prefix}MachEmu.${postfix})
  set(stripLib COMMAND strip -s ${sdkDir}/bin/${prefix}MachEmu.${postfix}${versionExt})

  if(enablePythonModule)
	    set(pythonModule MachEmuPy.cpython-${Python_VERSION_MAJOR}${Python_VERSION_MINOR}-x86_64-linux-gnu.so)
	    set(testControllersModule TestControllersPy.cpython-${Python_VERSION_MAJOR}${Python_VERSION_MINOR}-x86_64-linux-gnu.so)
  endif()
endif()

if(enablePythonModule)
	set(copyPythonModule COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/${pythonModule} ${sdkDir}/bin/${pythonModule})
	set(copyTestControllersModule COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/${testControllersModule} ${sdkDir}/bin/${testControllersModule})
  set(findPythonPackage "find_package(Python REQUIRED COMPONENTS Interpreter Development.Module)\n")
  set(installPythonModule "install(FILES \${CMAKE_SOURCE_DIR}/bin/${pythonModule} DESTINATION lib)\n")
endif()

# write the top level sdk cmake make list
file(WRITE ${CMAKE_BINARY_DIR}/${target_name}/RootCMakeLists.txt
  "# Copyright (c) 2021-2024 Nicolas Beddows <nicolas.beddows@gmail.com>\n\n"
  "# Permission is hereby granted, free of charge, to any person obtaining a copy\n"
  "# of this software and associated documentation files (the \"Software\"), to deal\n"
  "# in the Software without restriction, including without limitation the rights\n"
  "# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell\n"
  "# copies of the Software, and to permit persons to whom the Software is\n"
  "# furnished to do so, subject to the following conditions:\n\n"
  "# The above copyright notice and this permission notice shall be included in all\n"
  "# copies or substantial portions of the Software.\n\n"
  "# THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR\n"
  "# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\n"
  "# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE\n"
  "# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER\n"
  "# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,\n"
  "# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE\n"
  "# SOFTWARE.\n\n"
  "cmake_minimum_required (VERSION 3.28)\n"
  "#set(CMAKE_CXX_COMPILER g++-13)\n"
  "set(CMAKE_CXX_STANDARD 20)\n"
  "set(CMAKE_CXX_STANDARD_REQUIRED ON)\n"
  "set(CMAKE_CXX_EXTENSIONS OFF)\n"
  "set(include_dir include)\n"
  "set(module_dir module)\n"
  "set(source_dir source)\n"
  "project(mach-emu)\n"
  "find_library(libMachEmu MachEmu PATHS \${CMAKE_SOURCE_DIR}/bin/)\n"
  "find_package(nlohmann_json REQUIRED)\n"
  ${findPythonPackage}
  ${buildModules}
  "include(\${CMAKE_SOURCE_DIR}/CMakeIncludes/addTarget.cmake)\n"
  "add_subdirectory(Tests)\n"
  ${installPythonModule}
  "install(FILES \${CMAKE_SOURCE_DIR}/bin/${prefix}MachEmu.${postfix}${versionExt} DESTINATION lib)\n"
  ${installLinkLib}
  "install(DIRECTORY \${CMAKE_SOURCE_DIR}/Base/include/Base DESTINATION include)\n"
  "install(DIRECTORY \${CMAKE_SOURCE_DIR}/Controller/include/Controller DESTINATION include)\n"
  "install(DIRECTORY \${CMAKE_SOURCE_DIR}/Machine/include/Machine DESTINATION include)\n"
  ${installSoLink}
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
  ${copyPythonModule}
  ${copyTestControllersModule}
  ${copyLib}
  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/${prefix}MachEmu.${postfix}${versionExt} ${sdkDir}/bin/${prefix}MachEmu.${postfix}${versionExt}
  ${soLink}
  ${stripLib}
  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/CHANGELOG.md ${sdkDir}/CHANGELOG.md
  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/LICENSE.md ${sdkDir}/LICENSE.md
  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/README.md ${sdkDir}/README.md
  COMMAND ${CMAKE_COMMAND} -E chdir ${sdkDir} doxygen ${CMAKE_SOURCE_DIR}/Docs/Doxyfile
  COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different ${CMAKE_SOURCE_DIR}/Docs/images ${sdkDir}/Docs/html/Docs/images
  COMMAND ${CMAKE_COMMAND} -E tar -czf ${sdkDir}-v${CMAKE_PROJECT_VERSION}-${CMAKE_SYSTEM}-${CMAKE_SYSTEM_PROCESSOR}-dev.tar.gz ${sdkDir}
  COMMAND ${CMAKE_COMMAND} -E rm -rf ${sdkDir}
)

if(enablePythonModule)
  add_dependencies(${target_name} MachEmuPy)
else()
  add_dependencies(${target_name} MachEmu)
endif()
