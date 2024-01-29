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

function(add_target name type include_files module_files resource_files source_files)
    SOURCE_GROUP("Include Files" FILES ${include_files})
    SOURCE_GROUP("Module Files" FILES ${module_files})
    SOURCE_GROUP("Resource Files" FILES ${resource_files})
    SOURCE_GROUP("Source Files" FILES ${source_files})

    set(codeFiles ${include_files} ${module_files} ${resource_files} ${source_files})

    if(${type} MATCHES EXE)
        add_executable(${name} ${codeFiles})
    else()
        add_library(${name} ${type} ${codeFiles})
    endif()

    if(CMAKE_COMPILER_IS_GNUCXX)
        if(NOT ${type} MATCHES INTERFACE)
            if(${type} MATCHES EXE)
                set(cachePath ${CMAKE_BINARY_DIR}/Tests/gcm.cache)
            else()
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

            add_dependencies(${name} ${name}ModuleSymlink)
            target_compile_options(${name} PRIVATE -x c++ -fmodules-ts)
        endif()
    endif()
endfunction()