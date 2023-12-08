cmake_minimum_required (VERSION 3.25)
project(mach-emu)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_COMPILER_IS_GNUCXX)
    # Enable module compilation, don't rely on file ext
    # for compilation since we are .ixx ext which is not
    # recognised (using for msvc compatibility)
    set(CMAKE_CXX_FLAGS "-c -x c++ -fmodules-ts")
    # our required std modules
    set(STD_MODULES
        array
        bitset
        chrono
        cstdint
        filesystem
        functional
        memory
        string_view
        thread
        vector
    )

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
endif()

function(add_target name module_files source_files)
    SOURCE_GROUP("Module Files" FILES ${module_files})
    SOURCE_GROUP("Source Files" FILES ${source_files})

    add_library(${name}
        ${module_files}
        ${source_files}
    )

    if(CMAKE_COMPILER_IS_GNUCXX)
        # Workaround for not being able to set the search path
        # for the modules. We create a symlink to it from the
        # current directory.
        add_custom_target(
            ${name}ModuleSymlink
            COMMAND ${CMAKE_COMMAND} -E create_symlink
            ${CMAKE_BINARY_DIR}/gcm.cache
            gcm.cache
        )

        add_dependencies(${name} ${name}ModuleSymlink)
    endif()
endfunction()

set(module_dir module)
set(source_dir source)
add_subdirectory(Base)
add_subdirectory(Controller)
#add_subdirectory(Cpu)
add_subdirectory(CpuClock)
#add_subdirectory(Machine)
add_subdirectory(SystemBus)
#add_subdirectory(Tests)
