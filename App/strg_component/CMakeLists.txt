### Storage Engine Cmake

file( GLOB strg_comp_srcs "src/*.cpp" )
include_directories (include)

add_library( storage_engine SHARED  ${strg_comp_srcs} )

target_include_directories(storage_engine PUBLIC include ${CMAKE_SOURCE_DIR}/App/common/include)
target_link_libraries(storage_engine PUBLIC
	sqlite3
    log4c)