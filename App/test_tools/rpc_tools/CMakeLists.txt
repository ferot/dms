#RPC test tool

file( GLOB rpc_tools_srcs "src/*.cpp" )
include_directories(include)


find_library(JsonRpcCppCommon_LIB "jsonrpccpp-common")
find_library(JsonRpcCppClient_LIB "jsonrpccpp-client")
find_library(JsonCpp_LIB "jsoncpp")
find_library(microhttpd_LIB "microhttpd")
find_library(curl_LIB "curl")

add_executable( rpc_test_tool ${rpc_tools_srcs} )
target_include_directories( rpc_test_tool PUBLIC ${CMAKE_SOURCE_DIR}/common/include include)
target_link_libraries( rpc_test_tool LINK_PUBLIC 
	${JsonRpcCppCommon_LIB}
    ${JsonRpcCppClient_LIB}
    ${JsonCpp_LIB}
    ${microhttpd_LIB}
    ${curl_LIB}
    log4c
 )