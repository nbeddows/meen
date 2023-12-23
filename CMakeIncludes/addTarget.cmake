function(add_target name type module_files source_files)
    SOURCE_GROUP("Module Files" FILES ${module_files})
    SOURCE_GROUP("Source Files" FILES ${source_files})

    if(${type} MATCHES EXE)
        add_executable(${name}
            ${module_files}
            ${source_files}
    )
    else()
        add_library(${name} ${type}
            ${module_files}
            ${source_files}
        )
    endif()

    if(CMAKE_COMPILER_IS_GNUCXX)
        if(${type} MATCHES EXE)
            set(cachePath ${CMAKE_BINARY_DIR}/Tests/gcm.cache)
        else()
            target_compile_options(${name} PRIVATE -x c++)
            set(cachePath ${CMAKE_BINARY_DIR}/gcm.cache)
        endif()

        # Workaround for not being able to set the search path
        # for the modules. We create a symlink to it from the
        # current directory.
        add_custom_target(
            ${name}ModuleSymlink
            COMMAND ${CMAKE_COMMAND} -E create_symlink
            ${cachePath}
            gcm.cache
        )

        target_compile_options(${name} PRIVATE -x c++ -fmodules-ts)
        add_dependencies(${name} ${name}ModuleSymlink)
    endif()
endfunction()