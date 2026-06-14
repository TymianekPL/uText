cmake_minimum_required(VERSION 3.18)

project(Unicode)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(lib)
add_subdirectory(cli)

execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink
        ${CMAKE_BINARY_DIR}/compile_commands.json
        ${CMAKE_SOURCE_DIR}/compile_commands.json
    RESULT_VARIABLE _symlink_result
    ERROR_QUIET
)
if(NOT _symlink_result EQUAL 0)
    message(WARNING "Failed to create symlink for compile_commands.json. Some tools may not work correctly.")
endif()
