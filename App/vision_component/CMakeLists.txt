#Vision component
cmake_minimum_required (VERSION 2.6)

find_package( OpenCV REQUIRED )

include_directories(${OpenCV_INCLUDE_DIR} include)
file( GLOB vid_comp_srcs "src/*.cpp" )

add_library( vision_engine SHARED ${vid_comp_srcs} )

target_include_directories(vision_engine PUBLIC include ${CMAKE_SOURCE_DIR}/common/include)
target_link_libraries( vision_engine PUBLIC 
	${OpenCV_LIBS}
	log4c)