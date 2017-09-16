cmake_minimum_required (VERSION 2.6)
project (core_app)

add_definitions(-std=c++14 -Werror)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

add_subdirectory(comm_component)
add_subdirectory(core_component)
add_subdirectory(video_component)
add_subdirectory(strg_component)
add_subdirectory(test_tools)
add_subdirectory(common)

install (DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
DESTINATION "/usr")