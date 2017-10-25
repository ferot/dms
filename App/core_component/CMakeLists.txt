#Core component

file( GLOB core_comp_srcs "src/*.cpp" )
include_directories(include)

add_executable( ${PROJECT_NAME} ${core_comp_srcs} )
target_link_libraries( ${PROJECT_NAME} LINK_PUBLIC comm_engine storage_engine vision_engine common log4c pthread)
exec_program("mkdir -p bin")
exec_program("cp ../utils/log4crc bin")
