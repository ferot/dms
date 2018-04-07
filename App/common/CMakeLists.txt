#common component

file( GLOB common_srcs "src/*.cpp" )
include_directories (include)

find_library(JsonCpp_LIB "jsoncpp")


add_library( common SHARED  ${common_srcs} )

target_include_directories(common PUBLIC include ${CMAKE_SOURCE_DIR}/App/common/include)
target_link_libraries(common PUBLIC
    ${JsonCpp_LIB}
    log4c)