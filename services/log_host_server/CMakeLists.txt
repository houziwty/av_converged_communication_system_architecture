cmake_minimum_required(VERSION 3.5...3.16)

project(log_host_server VERSION "${BUILD_VERSION_STRING}" LANGUAGES CXX)
	
if(CMAKE_SYSTEM_NAME MATCHES "Linux")
	set(FILELOG_DIR ${MODULES_DIR}/libfilelog)
	set(XMQ_DIR ${MODULES_DIR}/libxmq)
	set(DEPENDS libboost_program_options.a libboost_filesystem.a libboost_json.a xmq filelog)
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
	set(BOOST_DIR ${3RDPART_DIR}/boost_1_78_0 ${3RDPART_DIR}/boost_1_78_0/stage/lib)
	set(FILELOG_DIR ${MODULES_DIR}/libfilelog ${ROOT_DIR}/build/x64/modules/libfilelog)
	set(XMQ_DIR ${MODULES_DIR}/libxmq ${ROOT_DIR}/build/x64/modules/libxmq)
		
	if(CMAKE_BUILD_TYPE MATCHES "Debug")
		set(DEPENDS 
			${BOOST_DIR}/libboost_program_options-vc143-mt-sgd-x64-1_78.lib 
			${BOOST_DIR}/libboost_filesystem-vc143-mt-sgd-x64-1_78.lib 
			${BOOST_DIR}/libboost_json-vc143-mt-sgd-x64-1_78.lib 
			${BOOST_DIR}/libboost_container-vc143-mt-sgd-x64-1_78.lib 
			${XMQ_DIR}/${CMAKE_BUILD_TYPE}/xmq.lib 
			${FILELOG_DIR}/${CMAKE_BUILD_TYPE}/filelog.lib)
	elseif(CMAKE_BUILD_TYPE MATCHES "Release")
		set(
			LINK_LIBS 
			${BOOST_DIR}/libboost_program_options-vc143-mt-s-x64-1_78.lib 
			${BOOST_DIR}/libboost_filesystem-vc143-mt-s-x64-1_78.lib 
			${BOOST_DIR}/libboost_json-vc143-mt-sgd-x64-1_78.lib 
			${BOOST_DIR}/libboost_container-vc143-mt-sgd-x64-1_78.lib 
			${XMQ_DIR}/${CMAKE_BUILD_TYPE}/xmq.lib 
			${FILELOG_DIR}/${CMAKE_BUILD_TYPE}/filelog.lib)
	endif()
	
	add_definitions(-DUSE_MODULE_FILE_LOG -DUSE_MODULE_NETWORK_XMQ)
endif()

include_directories(${INCLUDE_DIR} ${BOOST_DIR} ${XMQ_DIR} ${FILELOG_DIR})
file(GLOB_RECURSE src 
	"*.cpp" 
	"${SOURCE_DIR}/commandline/*.cpp" 
	"${SOURCE_DIR}/url/*.cpp" 
	"${SOURCE_DIR}/memory/*.cpp")

add_executable(${PROJECT_NAME} ${src})
target_link_libraries(${PROJECT_NAME} PRIVATE ${DEPENDS})