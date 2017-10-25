### Communication Engine Cmake

file( GLOB comm_comp_srcs "src/*.cpp" )
include_directories (include)

find_library(PahoMQTTlib_c "paho-mqtt3c")
find_library(PahoMQTTlib_a "paho-mqtt3a")

add_library( comm_engine SHARED  ${comm_comp_srcs} )

target_include_directories(comm_engine PUBLIC include ${CMAKE_SOURCE_DIR}/common/include)
target_link_libraries(comm_engine PUBLIC
	${PahoMQTTlib_a}
	${PahoMQTTlib_c}
    log4c)
