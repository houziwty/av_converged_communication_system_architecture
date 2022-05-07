cmake_minimum_required(VERSION 3.5...3.16)

project(av VERSION "${BUILD_VERSION_STRING}" LANGUAGES CXX)

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
	set(AVPARSER_DIR ${MODULES_DIR}/libavparser)
	set(AVPKT_DIR ${MODULES_DIR}/libavpkt)
	set(AVCODEC_DIR ${MODULES_DIR}/libavcodec)
	set(AVPLAYER_DIR ${MODULES_DIR}/libavplayer)
	set(DEPENDS libboost_thread.a avparser avpkt avcodec)
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
	set(BOOST_DIR ${3RDPART_DIR}/boost_1_78_0 ${3RDPART_DIR}/boost_1_78_0/stage/lib)
	set(AVPKT_DIR ${MODULES_DIR}/libavpkt ${ROOT_DIR}/build/x64/modules/libavpkt)
	set(AVPARSER_DIR ${MODULES_DIR}/libavparser ${ROOT_DIR}/build/x64/modules/libavparser)
	set(AVCODEC_DIR ${MODULES_DIR}/libavcodec ${ROOT_DIR}/build/x64/modules/libavcodec)
	set(AVPLAYER_DIR ${MODULES_DIR}/libavplayer ${ROOT_DIR}/build/x64/modules/libavplayer)
	
	if(CMAKE_BUILD_TYPE MATCHES "Debug")
		set(DEPENDS  
			${BOOST_DIR}/libboost_thread-vc143-mt-sgd-x64-1_78.lib
			${BOOST_DIR}/libboost_chrono-vc143-mt-sgd-x64-1_78.lib
			${AVPKT_DIR}/${CMAKE_BUILD_TYPE}/avpkt.lib 
			${AVPARSER_DIR}/${CMAKE_BUILD_TYPE}/avparser.lib 
			${AVCODEC_DIR}/${CMAKE_BUILD_TYPE}/avcodec.lib 
			${AVPLAYER_DIR}/${CMAKE_BUILD_TYPE}/avplayer.lib)
	elseif(CMAKE_BUILD_TYPE MATCHES "Release")
		set(DEPENDS 
			${BOOST_DIR}/libboost_thread-vc143-mt-s-x64-1_78.lib
			${BOOST_DIR}/libboost_chrono-vc143-mt-s-x64-1_78.lib 
			${AVPKT_DIR}/${CMAKE_BUILD_TYPE}/avpkt.lib 
			${AVPARSER_DIR}/${CMAKE_BUILD_TYPE}/avparser.lib 
			${AVCODEC_DIR}/${CMAKE_BUILD_TYPE}/avcodec.lib 
			${AVPLAYER_DIR}/${CMAKE_BUILD_TYPE}/avplayer.lib)
	endif()
	
	add_definitions(-DUSE_MODULE_AV_PARSER -DUSE_MODULE_AV_PKT -DUSE_MODULE_AV_CODEC -DUSE_MODULE_AV_PLAYER)
endif()

set(LIB_DIR ${MODULES_DIR}/libav)
include_directories(${INCLUDE_DIR} ${BOOST_DIR} ${AVPARSER_DIR} ${AVPKT_DIR} ${AVCODEC_DIR} ${AVPLAYER_DIR} ${LIB_DIR})
file(GLOB_RECURSE src "*.cpp")

add_library(${PROJECT_NAME} SHARED ${src})
target_link_libraries(${PROJECT_NAME} PRIVATE ${DEPENDS})