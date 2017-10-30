### Communication Engine Cmake

file( GLOB dispatch_comp_srcs "src/*.cpp" )
include_directories (include)


add_library( dispatch_engine SHARED  ${dispatch_comp_srcs} )

target_include_directories(dispatch_engine PUBLIC include ${CMAKE_SOURCE_DIR}/common/include
${CMAKE_SOURCE_DIR}/comm_component/include)
target_link_libraries(dispatch_engine PUBLIC
    log4c)
