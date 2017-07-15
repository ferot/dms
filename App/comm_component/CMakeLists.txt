file( GLOB comm_comp_srcs "src/*.cpp" )
include_directories (include)

find_library(JsonRpcCppCommon_LIB "jsonrpccpp-common")
find_library(JsonRpcCppServer_LIB "jsonrpccpp-server")
find_library(JsonCpp_LIB "jsoncpp")
find_library(microhttpd_LIB "microhttpd")

#create rpc-stubs and deploy them into proper locations
# TODO: resolve relative paths!!!
exec_program("jsonrpcstub '/home/tf/repos/dms/App/comm_component/files/spec.json' --cpp-server=AbstractStubServer --cpp-client=StubClient")
exec_program("mv abstractstubserver.h /home/tf/repos/dms/App/comm_component/include")
exec_program("mv stubclient.h /home/tf/repos/dms/App/comm_component/include")

add_library( comm_engine SHARED  ${comm_comp_srcs} )

target_include_directories(comm_engine PUBLIC include)
target_link_libraries(comm_engine PUBLIC
	${JsonRpcCppCommon_LIB}
    ${JsonRpcCppServer_LIB}
    ${JsonCpp_LIB}
    ${microhttpd_LIB})