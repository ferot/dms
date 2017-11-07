#Core component

file( GLOB core_comp_srcs "src/*.cpp" )
include_directories(include)

#Below path should be be parametrized!
set(CMAKE_PREFIX_PATH "/home/tf/qt/5.9.2/gcc_64/")

set(CMAKE_AUTOMOC ON)
find_package(Qt5Widgets)

add_executable( ${PROJECT_NAME} ${core_comp_srcs} )
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

