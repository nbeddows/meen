set(STD_MODULES_BUILD_TIMESTAMP_FILE std_modules_build_timestamp)
# One time build only
add_custom_target(StdModules ALL
    DEPENDS ${STD_MODULES_BUILD_TIMESTAMP_FILE}
)

set(STD_MODULES_OPTIONS -fmodules-ts -std=c++20 -c -x c++-system-header)

# compile the required std modules
add_custom_command(
    OUTPUT ${STD_MODULES_BUILD_TIMESTAMP_FILE}
    COMMAND ${CMAKE_COMMAND} -E echo "Building standard libraries modules"
    COMMAND ${CMAKE_CXX_COMPILER} ${STD_MODULES_OPTIONS} ${STD_MODULES}
    COMMAND ${CMAKE_COMMAND} -E touch ${STD_MODULES_BUILD_TIMESTAMP_FILE}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    BYPRODUCTS
)
