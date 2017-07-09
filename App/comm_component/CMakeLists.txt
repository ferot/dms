file( GLOB comm_comp_srcs "src/*.cpp" )
include_directories (include)

add_library( comm_engine SHARED  ${comm_comp_srcs} )

target_include_directories(comm_engine PUBLIC include)