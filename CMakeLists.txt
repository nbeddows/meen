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

# STAGE 1: set our required variables

cmake_minimum_required (VERSION 3.25)
set(include_dir include)
set(resource_dir resource)
set(source_dir source)
set(meen meen)
set(major 2)
set(minor 0)
set(bugfix 0)

if(${enable_rp2040} STREQUAL ON)
  include ($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)
endif()

project(${meen} VERSION ${major}.${minor}.${bugfix} LANGUAGES C CXX ASM)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_CXX_STANDARD 23)

if(${enable_rp2040} STREQUAL ON)
  add_executable(Pioasm IMPORTED)
  set_property(TARGET Pioasm PROPERTY IMPORTED_LOCATION $ENV{PICO_SDK_PATH}/build/pioasm/pioasm)
  set(Pioasm_FOUND 1)

  # In the 2.x SDK this is handled by picotool, need to account for this with version numbers
  add_executable(ELF2UF2 IMPORTED)
  set_property(TARGET ELF2UF2 PROPERTY IMPORTED_LOCATION $ENV{PICO_SDK_PATH}/build/elf2uf2/elf2uf2)
  set(ELF2UF2_FOUND 1)

  pico_sdk_init()
endif()

if(DEFINED MSVC)
  set(build_type $<CONFIG>)
else()
  set(build_type ${CMAKE_BUILD_TYPE})
endif()

if(NOT DEFINED build_arch)
  set(build_arch ${CMAKE_SYSTEM_PROCESSOR})
endif()

if(NOT DEFINED build_os)
  set(build_os ${CMAKE_SYSTEM_NAME})
endif()

if(NOT DEFINED archive_dir)
  set(archive_dir "lib")
endif()

if(NOT DEFINED runtime_dir)
  set(runtime_dir "bin")
endif()

if(${BUILD_SHARED_LIBS} STREQUAL ON)
  set(lib_type SHARED)

  if(DEFINED WIN32)
    set(ext dll)
  else()
    set(ext so)
  endif()
else()
  set(lib_type STATIC)

  if(DEFINED WIN32)
    set(ext lib)
    set(${meen}_${lib_type} ${meen}_${lib_type})
  else()
    set(ext a)
  endif()
endif()

# TODO: rp2040 does not output all artifacts correctly when setting this - investigate me
if(NOT ${enable_rp2040} STREQUAL ON)
  set(artifacts_dir $<1:${CMAKE_SOURCE_DIR}/artifacts/${build_type}/${build_arch}>)
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${artifacts_dir}/${archive_dir})
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${artifacts_dir}/${runtime_dir})
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${artifacts_dir}/${runtime_dir})
endif()

# Stage 2: locate our required packages

if(${enable_python_module} STREQUAL ON)
  find_package(Python REQUIRED COMPONENTS Interpreter Development.Module)
  find_package(pybind11 REQUIRED)
endif()

if(${enable_zlib} STREQUAL ON)
  find_package(ZLIB REQUIRED)
endif()

if(${enable_base64} STREQUAL ON)
  find_package(base64 REQUIRED)
endif()

if(${enable_hash_library} STREQUAL ON)
  find_package(hash-library REQUIRED)
endif()

if(${build_os} STREQUAL "baremetal")
  find_package(ArduinoJson REQUIRED)

  if(NOT BUILD_TESTING STREQUAL OFF)
    find_package(Unity REQUIRED)
  endif()
else()
  find_package(nlohmann_json REQUIRED)

  if(NOT BUILD_TESTING STREQUAL OFF)  
    find_package(GTest REQUIRED)
  endif()
endif()

# Stage 3: assign our include, source and resource files and assign to approriate groups

set(${meen}_public_include_files
  ${include_dir}/meen/Base.h
  ${include_dir}/meen/Error.h
  ${include_dir}/meen/IController.h
  ${include_dir}/meen/IMachine.h
  ${include_dir}/meen/MachineFactory.h
)

SOURCE_GROUP(${include_dir} FILES ${${meen}_public_include_files})

set(clock_include_files
  ${include_dir}/meen/clock/CpuClock.h
  ${include_dir}/meen/clock/CpuClockFactory.h
  ${include_dir}/meen/clock/ICpuClock.h
)

set(cpu_include_files
  ${include_dir}/meen/cpu/8080.h
  ${include_dir}/meen/cpu/CpuFactory.h
  ${include_dir}/meen/cpu/ICpu.h
)

set(machine_include_files ${include_dir}/meen/machine/Machine.h)

if(${enable_python_module} STREQUAL ON)
  set(${meen}_py_include_files
    ${include_dir}/meen/machine_py/ControllerPy.h
    ${include_dir}/meen/machine_py/MachineHolder.h
  )
endif()

set(opt_include_files
  ${include_dir}/meen/opt/Opt.h
)

set(utils_include_files
  ${include_dir}/meen/utils/ErrorCode.h
  ${include_dir}/meen/utils/Utils.h
)

set(${meen}_private_include_files
  ${clock_include_files}
  ${cpu_include_files}
  ${machine_include_files}
  ${opt_include_files}
  ${utils_include_files}
)

SOURCE_GROUP(${include_dir}/clock FILES ${clock_include_files})
SOURCE_GROUP(${include_dir}/cpu FILES ${cpu_include_files})
SOURCE_GROUP(${include_dir}/machine FILES ${machine_include_files})
SOURCE_GROUP(${include_dir}/machine_py FILES ${machine_py_include_files})
SOURCE_GROUP(${include_dir}/opt FILES ${opt_include_files})
SOURCE_GROUP(${include_dir}/utils FILES ${utils_include_files})

set(clock_source_files
  ${source_dir}/clock/CpuClock.cpp
  ${source_dir}/clock/CpuClockFactory.cpp
)

set(cpu_source_files
  ${source_dir}/cpu/8080.cpp
  ${source_dir}/cpu/CpuFactory.cpp
)

set(machine_source_files
  ${source_dir}/machine/Machine.cpp
  ${source_dir}/machine/MachineFactory.cpp
)

if(${enable_python_module} STREQUAL ON)
  set(${meen}_py_source_files
    ${source_dir}/machine_py/ControllerPy.cpp
    ${source_dir}/machine_py/MachineHolder.cpp
    ${source_dir}/machine_py/MachineModule.cpp
  )
endif()

set(opt_source_files
  ${source_dir}/opt/Opt.cpp
)

set(utils_source_files
  ${source_dir}/utils/ErrorCode.cpp
  ${source_dir}/utils/Utils.cpp
)

set(${meen}_source_files
  ${clock_source_files}
  ${cpu_source_files}
  ${machine_source_files}
  ${opt_source_files}
  ${utils_source_files}
)

SOURCE_GROUP(${source_dir}/clock FILES ${clock_source_files})
SOURCE_GROUP(${source_dir}/cpu FILES ${cpu_source_files})
SOURCE_GROUP(${source_dir}/machine FILES ${machine_source_files})
SOURCE_GROUP(${source_dir}/machine_py FILES ${machine_py_source_files})
SOURCE_GROUP(${source_dir}/opt FILES ${opt_source_files})
SOURCE_GROUP(${source_dir}/utils FILES ${utils_source_files})

if(DEFINED MSVC)
  include(${resource_dir}/generate_product_version.cmake)

  generate_product_version(
	  versionResource
	  BUNDLE "Machine Emulator ENgine"
	  COMPANY_NAME "Nicolas Beddows"
	  FILE_DESCRIPTION "Meen API"
	  NAME "Meen"
	  ORIGINAL_FILE_NAME "${lib_name}.dll"
	  PRODUCT_ICON ""
	  VERSION_MAJOR ${major}
	  VERSION_MINOR ${minor}
	  VERSION_PATCH ${bugfix}
	)

	set (${meen}_resource_files
		${versionResource}
	)

	SOURCE_GROUP(${resource_dir} FILES ${${meen}_resource_files})
endif()

# Stage 4: Create libraries and set their properties

add_library(${meen} ${lib_type}
  ${${meen}_private_include_files}
  ${${meen}_public_include_files}
  ${${meen}_resource_files}
  ${${meen}_source_files}
)

set_target_properties(${meen} PROPERTIES VERSION ${CMAKE_PROJECT_VERSION} SOVERSION ${CMAKE_PROJECT_VERSION})
target_compile_definitions(${meen} PRIVATE ${meen}_VERSION=\"${CMAKE_PROJECT_VERSION}\")
target_compile_definitions(${meen} PUBLIC ${meen}_${lib_type})
target_include_directories(${meen} PRIVATE ${include_dir})

if(${build_os} STREQUAL "baremetal")
  if(${enable_rp2040} STREQUAL ON)
    target_compile_definitions(${meen} PUBLIC ENABLE_MEEN_RP2040)
    target_link_libraries(${meen} PUBLIC pico_stdlib)
    target_link_libraries(${meen} PRIVATE pico_multicore)
  endif()

  target_link_libraries(${meen} PUBLIC ArduinoJson)

  if(DEFINED MSVC)
    message("todo: disable exceptions and rtti for msvc")
  else()
    target_compile_options(${meen} PRIVATE $<$<COMPILE_LANGUAGE:CXX>:-fno-exceptions -fno-rtti>)
  endif()
else()
  target_compile_definitions(${meen} PUBLIC ENABLE_NLOHMANN_JSON)
  target_link_libraries(${meen} PUBLIC nlohmann_json::nlohmann_json)
endif()

if(${enable_base64} STREQUAL ON)
  target_compile_definitions(${meen} PRIVATE ENABLE_BASE64)
  target_link_libraries(${meen} PRIVATE aklomp::base64)
endif()

if(${enable_hash_library} STREQUAL ON)
  target_compile_definitions(${meen} PRIVATE ENABLE_HASH_LIBRARY)
  target_link_libraries(${meen} PRIVATE hash-library::hash-library)
endif()

# check all the features required for save support, if the are enabled, then enable save support
if(${enable_base64} STREQUAL ON AND ${enable_hash_library} STREQUAL ON)
  target_compile_definitions(${meen} PRIVATE ENABLE_MEEN_SAVE)
endif()

if(${enable_zlib} STREQUAL ON)
  target_compile_definitions(${meen} PRIVATE ENABLE_ZLIB)
  target_link_libraries(${meen} PRIVATE ZLIB::ZLIB)
endif()

if(${enable_python_module} STREQUAL ON)
  pybind11_add_module(${meen}Py
    ${${meen}_py_include_files}
    ${${meen}_py_source_files}
  )
  target_link_directories(${meen}Py PRIVATE ${Python_LIBRARY_DIRS})
  target_link_libraries(${meen}Py PRIVATE pybind11::headers ${meen})
  install(TARGETS ${meen}Py DESTINATION ${runtime_dir})
endif()

if(DEFINED zlib_bin_dir)
  add_library(zlib SHARED IMPORTED)
  set_property(TARGET zlib PROPERTY IMPORTED_LOCATION ${zlib_bin_dir})
  install(IMPORTED_RUNTIME_ARTIFACTS zlib DESTINATION ./)
endif()

# Stage 5: Create unit tests

if(NOT BUILD_TESTING STREQUAL OFF)
  #add_subdirectory(Tests)

##### TEST CONTROLLERS

  set(test_controllers test_controllers)
  set(${test_controllers}_include_files
    tests/${include_dir}/${test_controllers}/BaseIoController.h
    tests/${include_dir}/${test_controllers}/CpmIoController.h
    tests/${include_dir}/${test_controllers}/MemoryController.h
    tests/${include_dir}/${test_controllers}/TestIoController.h
  )
  set(${test_controllers}_source_files
    tests/${source_dir}/${test_controllers}/BaseIoController.cpp
    tests/${source_dir}/${test_controllers}/CpmIoController.cpp
    tests/${source_dir}/${test_controllers}/TestIoController.cpp
  )

  if(${build_os} STREQUAL "baremetal")
    if(${enable_rp2040} STREQUAL ON)
      set(${test_controllers}_source_files
        ${${test_controllers}_source_files}
        tests/${source_dir}/${test_controllers}/RP2040MemoryController.cpp
        tests/${source_dir}/${test_controllers}/TestPrograms.S
      )
    endif()
  else()
    set(${test_controllers}_source_files
      ${${test_controllers}_source_files}
      tests/${source_dir}/${test_controllers}/MemoryController.cpp
    )
  endif()

  SOURCE_GROUP(${include_dir} FILES ${${test_controllers}_include_files})
  SOURCE_GROUP(${source_dir} FILES ${${test_controllers}_source_files})

  add_library(${test_controllers} STATIC ${${test_controllers}_include_files} ${${test_controllers}_source_files})

  set_target_properties(${test_controllers} PROPERTIES FOLDER tests)
  target_include_directories(${test_controllers} PRIVATE ${include_dir})
  target_include_directories(${test_controllers} PRIVATE tests/programs)
  target_include_directories(${test_controllers} PUBLIC tests/${include_dir})

##### TEST CONTROLLERS PYTHON

  if(${enable_python_module} STREQUAL ON)
    # todo: look into this relocation requirement, only required with python and shared enabled
    if(${lib_type} STREQUAL SHARED AND (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU"))
      target_compile_options(${test_controllers} PRIVATE -fPIC -Wno-attributes)
    endif()

    set (test_controllers_py test_controllers_py)
    set (${test_controllers_py}_source_files tests/${source_dir}/${test_controllers_py}/TestControllersPy.cpp)
    SOURCE_GROUP(${source_dir} FILES ${${test_controllers_py}_source_files})

    # todo: replace with TestControllersPy with ${test_controllers_py} when moving to meen 2.0
    pybind11_add_module(TestControllersPy ${${test_controllers_py}_source_files})

    set_target_properties(TestControllersPy PROPERTIES FOLDER tests)
    target_include_directories(TestControllersPy PRIVATE ${include_dir} tests/${include_dir})
    target_link_directories(TestControllersPy PRIVATE ${Python_LIBRARY_DIRS})
    target_link_libraries(TestControllersPy PRIVATE pybind11::headers ${test_controllers})
    install(TARGETS TestControllersPy LIBRARY DESTINATION ${runtime_dir})
    install(FILES ${CMAKE_SOURCE_DIR}/tests/source/test_controllers/BaseIoController.py DESTINATION tests)
    install(FILES ${CMAKE_SOURCE_DIR}/tests/source/test_controllers/CpmIoController.py DESTINATION tests)
    install(FILES ${CMAKE_SOURCE_DIR}/tests/source/test_controllers/MemoryController.py DESTINATION tests)
    install(FILES ${CMAKE_SOURCE_DIR}/tests/source/test_controllers/TestIoController.py DESTINATION tests)
  endif()

##### MEEN TEST

  if(${build_os} STREQUAL "baremetal")
    set(${meen}_test_source_files tests/${source_dir}/${meen}_test/MeenUnityTest.cpp)
    set(${meen}_test_deps unity::unity)
  else()
    set(${meen}_test_source_files tests/${source_dir}/${meen}_test/MeenGTest.cpp)
    set(${meen}_test_deps GTest::GTest)
  endif()

  SOURCE_GROUP(${source_dir} FILES ${${meen}_test_source_files})

  add_executable(${meen}_test ${${meen}_test_source_files})

  if(${build_os} STREQUAL "baremetal")
    if(${enable_rp2040} STREQUAL ON)
      pico_enable_stdio_usb(${meen}_test 1)
      pico_enable_stdio_uart(${meen}_test 0)
      pico_set_program_url(${meen}_test "https://github.com/neddows/mach-emu")
      pico_add_extra_outputs(${meen}_test)
      install(PROGRAMS ${CMAKE_BINARY_DIR}/${meen}_test.bin DESTINATION ./bin)
      install(FILES ${CMAKE_BINARY_DIR}/${meen}_test.uf2 DESTINATION ./bin)
      install(FILES ${CMAKE_BINARY_DIR}/${meen}_test.dis DESTINATION ./lib)
      install(FILES ${CMAKE_BINARY_DIR}/${meen}_test.elf.map DESTINATION ./lib)
      install(FILES ${CMAKE_BINARY_DIR}/${meen}_test.hex DESTINATION ./lib)
    endif()
  endif()

  if(${enable_python_module} STREQUAL ON)
    include(tests/pythonTestDeps.cmake)

    generatePythonUnitTestDeps(${artifacts_dir} ${CMAKE_SOURCE_DIR}/tests/source/test_controllers ${CMAKE_SOURCE_DIR}/tests/programs/ ${zlib_bin_dir})

    add_custom_command(
        TARGET ${meen}_test POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_BINARY_DIR}/MachineTestDeps${build_type}.py ${CMAKE_CURRENT_SOURCE_DIR}/tests/source/${meen_test}/MachineTestDeps.py
        )

    install(FILES ${CMAKE_SOURCE_DIR}/tests/source/meen_test/MachineTestDeps.py DESTINATION tests)
    install(FILES ${CMAKE_SOURCE_DIR}/tests/source/meen_test/test_Machine.py DESTINATION tests)
  endif()

  if(DEFINED MSVC)
    set_target_properties(${meen}_test PROPERTIES VS_DEBUGGER_COMMAND_ARGUMENTS "\"${CMAKE_SOURCE_DIR}/tests/programs/\"")
  endif()

  set_target_properties(${meen}_test PROPERTIES FOLDER tests)
  target_compile_definitions(${meen}_test PRIVATE PROGRAMS_DIR=\"programs/\")

  target_link_libraries(${meen}_test PRIVATE
    ${${meen}_test_deps}
    ${meen}
    ${test_controllers}
  )

  install(TARGETS ${meen}_test RUNTIME DESTINATION ${runtime_dir})

  if(${build_os} STREQUAL "baremetal")
    install(FILES ${CMAKE_SOURCE_DIR}/tests/programs/bdosMsg.bin DESTINATION tests/programs)
    install(FILES ${CMAKE_SOURCE_DIR}/tests/programs/exitTest.bin DESTINATION tests/programs)
    install(FILES ${CMAKE_SOURCE_DIR}/tests/programs/nopStart.bin DESTINATION tests/programs)
    install(FILES ${CMAKE_SOURCE_DIR}/tests/programs/nopEnd.bin DESTINATION tests/programs)
    install(FILES ${CMAKE_SOURCE_DIR}/tests/programs/8080EXM.COM DESTINATION tests/programs)
    install(FILES ${CMAKE_SOURCE_DIR}/tests/programs/8080PRE.COM DESTINATION tests/programs)
    install(FILES ${CMAKE_SOURCE_DIR}/tests/programs/CPUTEST.COM DESTINATION tests/programs)
    install(FILES ${CMAKE_SOURCE_DIR}/tests/programs/TST8080.COM DESTINATION tests/programs)
  else()
    install(DIRECTORY ${CMAKE_SOURCE_DIR}/tests/programs DESTINATION tests)
##### Generate unit test run script
    if(DEFINED MSVC)
      # write out a script which exports the runtime directory and runs the tests, $@ - bash all command line arguemnts
      if(${enable_python_module} STREQUAL ON)
        set(pythonFilter "set python-filter=-k *")
        set(pythonCmdLine "IF \"%~1\"==\"--python_filter\" set python-filter=-k %~2")
        set(pythonExecute "echo Running Python unit tests\npython tests\\test_Machine.py -v %python-filter%")
      endif()

      # start run-meen-unit-tests.bat --gtest_filter=*LXI_B* --python_filter=Tst8080
      file(WRITE ${CMAKE_BINARY_DIR}/run-${meen}-unit-tests.bat
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
echo Adding %cd%\\${runtime_dir} to PATH\n\
PATH=%cd%\\${runtime_dir};%PATH%\n\
echo Running C++ unit tests\n\
${runtime_dir}\\${meen}_test %gtest-filter% tests\\programs\\\n\
${pythonExecute}\n\
pause\n\
exit"
      )

      install(PROGRAMS ${CMAKE_BINARY_DIR}/run-${meen}-unit-tests.bat DESTINATION .)
    else()
      if(enable_python_module STREQUAL ON)
        set(pythonFilter "export python_filter=\"-k*\"")
        set(pythonCmdLine "--python_filter)\nexport python_filter=\"-k$2\"\nshift\n;;")
        set(pythonExecute "echo Running Python unit tests\npython tests/test_Machine.py -v \${python_filter}")
      endif()

      file(WRITE ${CMAKE_BINARY_DIR}/run-${meen}-unit-tests.sh
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
      echo ./run-${meen}-unit-tests [--gtest_filter \\\$gtest_filter][--python_filter \\\${python_filter}
      exit\n\
      ;;\n\
  esac\n\
  shift\n\
done\n\
echo Adding `pwd`/${runtime_dir} to LD_LIBRARY_PATH\n\
export LD_LIBRARY_PATH=`pwd`/${runtime_dir}:\${LD_LIBRARY_PATH}\n\
echo Running C++ unit tests\n\
${runtime_dir}/${meen}_test \"\${gtest_filter}\" tests/programs/\n\
${pythonExecute}"
      )

      install(PROGRAMS ${CMAKE_BINARY_DIR}/run-${meen}-unit-tests.sh DESTINATION .)
    endif()
  endif()
endif()

# Stage 6: Set CPack install parameters

set(CMAKE_INSTALL_PREFIX ./)
set(CPACK_PACKAGE_FILE_NAME ${meen}-v${CMAKE_PROJECT_VERSION}-${build_os}-${build_arch}-${CMAKE_C_COMPILER_ID}-${CMAKE_C_COMPILER_VERSION})
set(CPACK_GENERATOR TGZ)

if(DEFINED CMAKE_STRIP)
  add_custom_target(${meen}_strip_pkg
    COMMAND ${CMAKE_COMMAND} -E tar -xzf ${CPACK_PACKAGE_FILE_NAME}.tar.gz
    COMMAND ${CMAKE_STRIP} -s ${CPACK_PACKAGE_FILE_NAME}/${runtime_dir}/${meen}_test
    COMMAND ${CMAKE_STRIP} -s ${CPACK_PACKAGE_FILE_NAME}/${archive_dir}/lib${meen}.${ext}
    COMMAND ${CMAKE_COMMAND} -E tar czf ${CPACK_PACKAGE_FILE_NAME}.tar.gz ${CPACK_PACKAGE_FILE_NAME}/
    COMMAND ${CMAKE_COMMAND} -E rm -r ${CPACK_PACKAGE_FILE_NAME}/
  )
endif()

target_sources(${meen} INTERFACE FILE_SET HEADERS BASE_DIRS ${include_dir} FILES ${${meen}_public_include_files})

install(CODE "execute_process(COMMAND doxygen ${CMAKE_SOURCE_DIR}/docs/Doxyfile)")
install(CODE "execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/docs/images ${CMAKE_BINARY_DIR}/docs/html/docs/images)")
install(CODE "execute_process(COMMAND cppcheck -ibuild -i_CPack_Packages -itests --inline-suppr --enable=warning,style,performance,portability,unusedFunction --std=c++20 ${CMAKE_SOURCE_DIR})")
install(TARGETS ${meen} FILE_SET HEADERS)
install(DIRECTORY ${CMAKE_BINARY_DIR}/docs DESTINATION .)
install(FILES ${CMAKE_SOURCE_DIR}/CHANGELOG.md DESTINATION .)
install(FILES ${CMAKE_SOURCE_DIR}/LICENSE.md RENAME LICENSE DESTINATION .)
install(FILES ${CMAKE_SOURCE_DIR}/README.md DESTINATION .)

if(NOT DEFINED WIN32)
  set(CPACK_SET_DESTDIR ON)
endif()

include(CPack)