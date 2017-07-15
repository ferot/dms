cmake_minimum_required (VERSION 2.6)

find_package( OpenCV REQUIRED )

include_directories(${OpenCV_INCLUDE_DIR} include)
file( GLOB vid_comp_srcs "src/*.cpp" )

add_executable( tracker ${vid_comp_srcs} )
target_link_libraries( tracker ${OpenCV_LIBS} )