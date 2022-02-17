cmake_minimum_required(VERSION 3.5...3.16)

project(av VERSION "${BUILD_VERSION_STRING}" LANGUAGES CXX)

set(
	AVPARSER_ROOT_DIR 
	${ROOT_DIR}/modules/libavparser)
set(
	AVPKT_ROOT_DIR 
	${ROOT_DIR}/modules/libavpkt)
if(CMAKE_SYSTEM_NAME MATCHES "Linux")
	link_directories(${AVPARSER_ROOT_DIR})
	set(
		LINK_LIBS 
		libboost_thread.a 
		avparser 
		avpkt)
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
	set(
		BOOST_ROOT_DIR 
		${3RDPART_DIR}/boost_1_78_0)
	set(
		LINK_DIR 
		${BOOST_ROOT_DIR}/stage/lib)
	set(
		AVPKT_LINK_DIR 
		${ROOT_DIR}/build/win32/modules/libavpkt/${CMAKE_BUILD_TYPE})
	set(
		AVPARSER_LINK_DIR 
		${ROOT_DIR}/build/win32/modules/libavparser/${CMAKE_BUILD_TYPE})
	if(CMAKE_BUILD_TYPE MATCHES "Debug")
		set(
			LINK_LIBS 
			${LINK_DIR}/libboost_thread-vc142-mt-sgd-x64-1_78.lib
			${LINK_DIR}/libboost_chrono-vc142-mt-sgd-x64-1_78.lib
			${AVPKT_LINK_DIR}/avpkt.lib 
			${AVPARSER_LINK_DIR}/avparser.lib)
	elseif(CMAKE_BUILD_TYPE MATCHES "Release")
		set(
			LINK_LIBS 
			${LINK_DIR}/libboost_thread-vc142-mt-s-x64-1_78.lib
			${LINK_DIR}/libboost_chrono-vc142-mt-s-x64-1_78.lib 
			${AVPKT_LINK_DIR}/avpkt.lib 
			${AVPARSER_LINK_DIR}/avparser.lib)
	endif()
	add_definitions(-DUSE_MODULE_AV_PARSER -DUSE_MODULE_AV_PKT)
endif()

set(LIB_ROOT_DIR ${ROOT_DIR}/modules/libav)
include_directories(${INCLUDE_DIR} ${LIB_ROOT_DIR} ${BOOST_ROOT_DIR} ${AVPARSER_ROOT_DIR} ${AVPKT_ROOT_DIR})
file(GLOB_RECURSE main "*.cpp")

add_library(
	${PROJECT_NAME} 
	SHARED 
	${main})
target_link_libraries(
	${PROJECT_NAME} 
	PRIVATE 
	${LINK_LIBS})