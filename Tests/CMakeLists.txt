include(FetchContent)
FetchContent_Declare(
  googletest
  # Specify the commit you depend on and update it regularly.
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

if(CMAKE_COMPILER_IS_GNUCXX)
  # our required std modules for the unit tests
  set(STD_MODULES cstdint memory string vector)

  # One time build only
  add_custom_target(TestStdModules ALL
    DEPENDS ${STD_MODULES_BUILD_TIMESTAMP_FILE}
  )

  # compile the required std modules
  add_custom_command(
    OUTPUT ${STD_MODULES_BUILD_TIMESTAMP_FILE}
    COMMAND ${CMAKE_COMMAND} -E echo "Building standard libraries modules for tests"
    COMMAND ${CMAKE_CXX_COMPILER} ${STD_MODULES_OPTIONS} ${STD_MODULES}
    COMMAND ${CMAKE_COMMAND} -E touch ${STD_MODULES_BUILD_TIMESTAMP_FILE}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/Tests
    BYPRODUCTS
  )

  # copy out the required modules
  add_custom_target(
    TestModules
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_BINARY_DIR}/gcm.cache/Base.gcm
    ${CMAKE_BINARY_DIR}/gcm.cache/IController.gcm
    ${CMAKE_BINARY_DIR}/gcm.cache/IMachine.gcm
    ${CMAKE_BINARY_DIR}/gcm.cache/MachineFactory.gcm
    ${CMAKE_BINARY_DIR}/Tests/TestControllers/gcm.cache/BaseIoController.gcm
    ${CMAKE_BINARY_DIR}/Tests/TestControllers/gcm.cache/CpmIoController.gcm
    ${CMAKE_BINARY_DIR}/Tests/TestControllers/gcm.cache/MemoryController.gcm
    ${CMAKE_BINARY_DIR}/Tests/TestControllers/gcm.cache/TestIoController.gcm
    ${CMAKE_BINARY_DIR}/Tests/gcm.cache
  )
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
add_subdirectory(ControllerTest)
add_subdirectory(MachineTest)
add_subdirectory(TestControllers)
set_target_properties(ControllerTest PROPERTIES FOLDER "Tests")
set_target_properties(MachineTest PROPERTIES FOLDER "Tests")
set_target_properties(TestControllers PROPERTIES FOLDER "Tests")

if(CMAKE_COMPILER_IS_GNUCXX)
  add_dependencies(ControllerTest TestModules)
  add_dependencies(MachineTest TestModules)
endif()
