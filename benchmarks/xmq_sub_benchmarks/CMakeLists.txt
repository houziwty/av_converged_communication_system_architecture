cmake_minimum_required(VERSION 3.5...3.16)

project(xmq_sub_benchmarks VERSION "${BUILD_VERSION_STRING}" LANGUAGES CXX)

set(
	XMQ_ROOT_DIR 
	${ROOT_DIR}/modules/libxmq)
	
if(CMAKE_SYSTEM_NAME MATCHES "Linux")
	link_directories(${XMQ_ROOT_DIR} ${LOG_ROOT_DIR})
	set(
		LINK_LIBS 
		libboost_thread.a
		pthread 
		xmq)
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
	set(
		BOOST_ROOT_DIR 
		${3RDPART_DIR}/boost_1_78_0)
	set(
		BOOST_LINK_DIR 
		${BOOST_ROOT_DIR}/stage/lib)
	set(
		XMQ_LINK_DIR 
		${ROOT_DIR}/build/x64/modules/libxmq/${CMAKE_BUILD_TYPE})
	if(CMAKE_BUILD_TYPE MATCHES "Debug")
		set(
			LINK_LIBS 
			${BOOST_LINK_DIR}/libboost_thread-vc143-mt-sgd-x64-1_78.lib 
			${BOOST_LINK_DIR}/libboost_chrono-vc143-mt-sgd-x64-1_78.lib 
			${BOOST_LINK_DIR}/libboost_program_options-vc143-mt-sgd-x64-1_78.lib	
			${BOOST_LINK_DIR}/libboost_serialization-vc143-mt-sgd-x64-1_78.lib 		
			${XMQ_LINK_DIR}/xmq.lib)
	elseif(CMAKE_BUILD_TYPE MATCHES "Release")
		set(
			LINK_LIBS 
			${BOOST_LINK_DIR}/libboost_thread-vc143-mt-s-x64-1_78.lib 
			${BOOST_LINK_DIR}/libboost_chrono-vc143-mt-s-x64-1_78.lib 
			${BOOST_LINK_DIR}/libboost_program_options-vc143-mt-s-x64-1_78.lib	
			${BOOST_LINK_DIR}/libboost_serialization-vc143-mt-s-x64-1_78.lib 		
			${XMQ_LINK_DIR}/xmq.lib)
	endif()
	add_definitions(-DUSE_MODULE_FILE_LOG -DUSE_MODULE_NETWORK_XMQ)
endif()

set(TEST_XMQ_SUB_DIR ${ROOT_DIR}/benchmarks/xmq_sub_benchmarks)
include_directories(
	${INCLUDE_DIR} 
	${TEST_XMQ_SUB_DIR} 
	${BOOST_ROOT_DIR} 
	${XMQ_ROOT_DIR})

file(GLOB_RECURSE main "*.cpp")
file(GLOB_RECURSE utils 
	"${SOURCE_DIR}/time/*.cpp" 
	"${SOURCE_DIR}/url/*.cpp" 
	"${SOURCE_DIR}/memory/*.cpp")

add_executable(
	${PROJECT_NAME} 
	${main} 
	${utils})
target_link_libraries(
	${PROJECT_NAME} 
	PRIVATE 
	${LINK_LIBS})