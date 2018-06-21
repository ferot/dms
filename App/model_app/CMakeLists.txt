#model_app

file( GLOB core_comp_srcs "src/*.cpp" )
include_directories(include)

set( APP_NAME model_app)

#Below path should be be parametrized!
set(CMAKE_PREFIX_PATH "/home/tf/qt/5.9.2/gcc_64/")
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set (GUI_SRC
    src/window.cpp
    src/window.h
)

set (MODEL_SRC 
	include/ModelEngine.h
	src/ModelEngine.cpp
	)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)

add_executable( ${APP_NAME} ${core_comp_srcs} ${MODEL_SRC} ${GUI_SRC} )
#needed because of default set std to C++11, what caused error on compiling C++14 srcs
set_property(TARGET ${APP_NAME} PROPERTY CXX_STANDARD 14)


target_link_libraries( ${APP_NAME} LINK_PUBLIC 
comm_engine 
dispatch_engine 
vision_engine
common
Qt5::Widgets
log4c
fann 
pthread
storage_engine)

