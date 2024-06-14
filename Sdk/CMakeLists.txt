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
set(sdkDir ${CMAKE_BINARY_DIR}/${target_name}/${libMachEmu})

if(MSVC)
  set(scriptExt .bat)
  set(postfix .dll)
  set(exeExt .exe)
  set(copyLib COMMAND ${CMAKE_COMMAND} -E copy_if_different ${artifactsDir}/${archiveDir}/${libMachEmu}.lib ${sdkDir}/${archiveDir}/${libMachEmu}.lib)

  if(enablePythonModule)
    set(pythonModule ${libMachEmu}Py.cp${Python_VERSION_MAJOR}${Python_VERSION_MINOR}-win_amd64.pyd)
  endif()

  # write out a script which exports the runtime directory and runs the tests, $@ - bash all command line arguemnts
  if(NOT BUILD_TESTING STREQUAL OFF)
    if(enablePythonModule)
      set(testControllersModule TestControllersPy.cp${Python_VERSION_MAJOR}${Python_VERSION_MINOR}-win_amd64.pyd)
      set(pythonFilter "set python-filter=-k *")
      set(pythonCmdLine "IF \"%~1\"==\"--python_filter\" set python-filter=-k %~2")
      set(pythonExecute 
        "echo Running Python unit tests\n\
        python tests\\test_Machine.py -v %python-filter%"
      )
    endif()

    # start run-mach_emu-tests.bat --gtest_filter=*LXI_B* --python_filter=Tst8080
    file(WRITE ${CMAKE_BINARY_DIR}/${target_name}/run-${libMachEmu}-tests${scriptExt}
        "@echo off\n\
set gtest-filter=--gtest_filter=*\n\
${pythonFilter}
:parse\n\
IF \"%~1\"==\"\" GOTO endparse\n\
IF \"%~1\"==\"--gtest_filter\" set gtest-filter=%~1=%~2\n\
${pythonCmdLine}\n\
SHIFT\n\
GOTO parse\n\
:endparse\n\
echo Adding %cd%\\${runtimeDir} to PATH\n\
PATH=%cd%\\${runtimeDir};%PATH%\n\
echo Running C++ unit tests\n\
tests\\MachineTest %gtest-filter% tests\\Programs\\\n\
${pythonExecute}
pause\n\
exit"
    )
  endif()
else()
  set(scriptExt .sh)
  set(postfix .so)
  set(prefix lib)
  set(versionExt .${machEmuVersion})

  if(${CMAKE_SYSTEM_PROCESSOR} STREQUAL aarch64)
    set(stripLib COMMAND aarch64-linux-gnu-strip -s ${sdkDir}/${runtimeDir}/${prefix}${libMachEmu}${postfix}${versionExt})
  elseif(${CMAKE_SYSTEM_PROCESSOR} STREQUAL armv7hf)
    set(stripLib COMMAND arm-linux-gnueabihf-strip -s ${sdkDir}/${runtimeDir}/${prefix}${libMachEmu}${postfix}${versionExt})
  else()
    set(stripLib COMMAND strip -s ${sdkDir}/${runtimeDir}/${prefix}${libMachEmu}${postfix}${versionExt})
  endif()

  if(enablePythonModule)
    set(pythonModule ${libMachEmu}Py.cpython-${Python_VERSION_MAJOR}${Python_VERSION_MINOR}-x86_64-linux-gnu.so)
  endif()

  if(NOT BUILD_TESTING STREQUAL OFF)
    if(enablePythonModule)
      set(testControllersModule TestControllersPy.cpython-${Python_VERSION_MAJOR}${Python_VERSION_MINOR}-x86_64-linux-gnu.so)
      set(pythonFilter "export python_filter=\"-k*\"")
      set(pythonCmdLine
        "--python_filter)\n\
            export python_filter=\"-k$2\"\n\
            shift\n\
            ;;"
      )
      set(pythonExecute
        "echo Running Python unit tests\n\
        python tests/test_Machine.py -v \"\${python_filter}\""
      )
    endif()

    set(chmod COMMAND chmod 755 ${sdkDir}/run-${libMachEmu}-tests${scriptExt})

    file(WRITE ${CMAKE_BINARY_DIR}/${target_name}/run-${libMachEmu}-tests${scriptExt}
"#!/bin/bash\n\
export gtest_filter=\"--gtest_filter=*\"\n\
${pythonFilter}\n\
while [[ \$# -gt 0 ]]; do\n\
    case \$1 in\n\
        --gtest_filter)\n\
            export gtest_filter=\"\$1=\$2\"\n\
            shift\n\
            ;;\n\
        ${pythonCmdLine}\n\
        *)\n\
            echo Invalid option: \$1\n\
            echo ./run-machine-tests [--gtest_filter \\\$gtest_filter][--python_filter \\\${python_filter}
            exit\n\
            ;;\n\
    esac\n\
    shift\n\
done\n\
echo Adding `pwd`/${runtimeDir} to LD_LIBRARY_PATH\n\
export LD_LIBRARY_PATH=`pwd`/${runtimeDir}:\${LD_LIBRARY_PATH}\n\
echo Running C++ unit tests\n\
tests/MachineTest \"\${gtest_filter}\" tests/Programs/\n\
${pythonExecute}"
    )
  endif()
endif()

if(enablePythonModule)
  set(copyPythonModule COMMAND ${CMAKE_COMMAND} -E copy_if_different ${artifactsDir}/${runtimeDir}/${pythonModule} ${sdkDir}/${runtimeDir}/${pythonModule})

  if(NOT BUILD_TESTING STREQUAL OFF)
    include(${CMAKE_SOURCE_DIR}/Tests/MachineTest/pythonTestDeps.cmake)

    if(DEFINED zlibBinDir)
      set(zlibPath ${runtimeDir})
    endif()
    
    generatePythonUnitTestDeps(. ./tests ./tests/Programs/ ${zlibPath})

    set(copyPythonUnitTests
      COMMAND ${CMAKE_COMMAND} -E copy_if_different ${artifactsDir}/${runtimeDir}/${testControllersModule} ${sdkDir}/${runtimeDir}/${testControllersModule}
      COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/Tests/MachineTest/${source_dir}/test_Machine.py ${sdkDir}/tests/test_Machine.py
      COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/Tests/TestControllers/${source_dir}/BaseIoController.py ${sdkDir}/tests/BaseIoController.py
      COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/Tests/TestControllers/${source_dir}/CpmIoController.py ${sdkDir}/tests/CpmIoController.py
      COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/Tests/TestControllers/${source_dir}/MemoryController.py ${sdkDir}/tests/MemoryController.py
      COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/Tests/TestControllers/${source_dir}/TestIoController.py ${sdkDir}/tests/TestIoController.py
      COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_BINARY_DIR}/Sdk/MachineTestDeps${buildType}.py ${sdkDir}/tests/MachineTestDeps.py
    )
  endif()
endif()

if(NOT BUILD_TESTING STREQUAL OFF)
  set(copyTests
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${artifactsDir}/${runtimeDir}/MachineTest${exeExt} ${sdkDir}/tests/MachineTest${exeExt}
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Tests/Programs ${sdkDir}/tests/Programs
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_BINARY_DIR}/Sdk/run-${libMachEmu}-tests${scriptExt} ${sdkDir}/run-${libMachEmu}-tests${scriptExt}
    ${chmod}
    ${copyPythonModule}
    ${copyPythonUnitTests}
  )
endif()

if(DEFINED zlibBinDir)
  set(copyZlib COMMAND ${CMAKE_COMMAND} -E copy_directory ${zlibBinDir} ${sdkDir}/${runtimeDir})
endif()

add_custom_target(
  ${target_name}
  COMMAND cppcheck -i build -i CMakeFiles -i Tests -i Utils --inline-suppr --enable=warning,style,performance,portability,unusedFunction --std=c++20 "${CMAKE_SOURCE_DIR}"
  COMMAND ${CMAKE_COMMAND} -E make_directory ${sdkDir}
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Base/include ${sdkDir}/${include_dir}
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Controller/include ${sdkDir}/${include_dir}
  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/Machine/include/Machine/IMachine.h ${sdkDir}/${include_dir}/Machine/IMachine.h
  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/Machine/include/Machine/MachineFactory.h ${sdkDir}/${include_dir}/Machine/MachineFactory.h
  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${artifactsDir}/${runtimeDir}/${prefix}${libMachEmu}${postfix}${versionExt} ${sdkDir}/${runtimeDir}/${prefix}${libMachEmu}${postfix}${versionExt}
  ${stripLib}
  ${copyLib}
  ${copyZlib}
  ${copyTests}
  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/CHANGELOG.md ${sdkDir}/CHANGELOG.md
  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/LICENSE.md ${sdkDir}/LICENSE.md
  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/README.md ${sdkDir}/README.md
  COMMAND ${CMAKE_COMMAND} -E chdir ${sdkDir} doxygen ${CMAKE_SOURCE_DIR}/docs/Doxyfile
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/docs/images ${sdkDir}/docs/html/docs/images
  COMMAND ${CMAKE_COMMAND} -E tar -czf ${sdkDir}-v${CMAKE_PROJECT_VERSION}-${CMAKE_SYSTEM}-${CMAKE_SYSTEM_PROCESSOR}-${runtimeDir}.tar.gz ${sdkDir}
  COMMAND ${CMAKE_COMMAND} -E rm -rf ${sdkDir}
)

if(enablePythonModule)
  add_dependencies(${target_name} ${libMachEmu}Py)
else()
  add_dependencies(${target_name} ${libMachEmu})
endif()
