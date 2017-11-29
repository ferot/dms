cmake_minimum_required (VERSION 2.8.11)
project (core_app)

add_definitions(-std=c++14 -Werror -Wall -Wpedantic)
set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

add_subdirectory(comm_component)
add_subdirectory(core_component)
add_subdirectory(vision_component)
add_subdirectory(dispatcher_component)
add_subdirectory(strg_component)
add_subdirectory(common)
add_subdirectory(calib_tools)

install (DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
DESTINATION "/usr")
