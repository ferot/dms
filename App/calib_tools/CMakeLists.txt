#Calib Tool

set(CALIB_APP calib_tool)

include_directories(calib_tool)
include_directories(calib_tool/include)

file( GLOB calib_tool_srcs "calib_tool/src/*.cpp" )

#Below path should be be parametrized!
set(CMAKE_PREFIX_PATH "/home/tf/qt/5.9.2/gcc_64/")
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)


find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(fann REQUIRED)

QT5_WRAP_UI(UI
	calib_tool/calibtool.ui
)

set (GUI_SRC
    calib_tool/main.cpp
    calib_tool/calibtool.cpp
    calib_tool/calibtool.h
)


add_executable( ${CALIB_APP} ${calib_tool_srcs} ${GUI_SRC} ${UI} )
#needed because of default set std to C++11, what caused error on compiling C++14 srcs
set_property(TARGET ${CALIB_APP} PROPERTY CXX_STANDARD 14)

target_include_directories( ${CALIB_APP} PUBLIC include)

target_link_libraries( ${CALIB_APP} LINK_PUBLIC 
common
comm_engine
dispatch_engine 
Qt5::Widgets
log4c
fann)

