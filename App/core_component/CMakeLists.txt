#Core component

file( GLOB core_comp_srcs "src/*.cpp" )
include_directories(include)

#Below path should be be parametrized!
set(CMAKE_PREFIX_PATH "/home/tf/qt/5.9.2/gcc_64/")
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set (GUI_SRC
    src/window.cpp
    src/window.h
)

set (VISION_SRC 
	src/visionEngineWrapper.h
	src/visionEngineWrapper.cpp
	)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)

add_executable( ${PROJECT_NAME} ${core_comp_srcs} ${GUI_SRC})
#needed because of default set std to C++11, what caused error on compiling C++14 srcs
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 14)


target_link_libraries( ${PROJECT_NAME} LINK_PUBLIC 
comm_engine 
storage_engine 
vision_engine 
dispatch_engine 
common 
Qt5::Widgets
log4c 
pthread)

#copy log4crc setting file to newly created dir
exec_program("mkdir -p bin")
exec_program("cp ../utils/log4crc bin")
exec_program("cp ../utils/config.ini bin")

