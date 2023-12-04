include(FetchContent)
FetchContent_Declare(
  googletest
  # Specify the commit you depend on and update it regularly.
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

#set_property(GLOBAL PROPERTY USE_FOLDERS ON)
add_subdirectory(ControllerTest)
add_subdirectory(MachineTest)
add_subdirectory(TestControllers)
set_target_properties(ControllerTest PROPERTIES FOLDER "Tests")
set_target_properties(MachineTest PROPERTIES FOLDER "Tests")
set_target_properties(TestControllers PROPERTIES FOLDER "Tests")