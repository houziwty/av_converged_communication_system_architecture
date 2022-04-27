cmake_minimum_required(VERSION 3.5...3.16)

project(avcodec VERSION "${BUILD_VERSION_STRING}" LANGUAGES CXX)

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,-Bsymbolic")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wl,-Bsymbolic")
	set(AVPKT_DIR ${MODULES_DIR}/libavpkt)
	set(DEPENDS libavcodec.a libavformat.a libavutil.a libswscale.a libswresample.a)
	add_definitions(-D__STDC_CONSTANT_MACROS)
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
	set(BOOST_DIR ${3RDPART_DIR}/boost_1_78_0 ${3RDPART_DIR}/boost_1_78_0/stage/lib)
	set(FFMPEG_DIR ${3RDPART_DIR}/ffmpeg/x64/ffmpeg-n5.0-latest-win64-gpl-shared-5.0/include ${3RDPART_DIR}/ffmpeg/x64/ffmpeg-n5.0-latest-win64-gpl-shared-5.0/lib)
	set(AVPKT_DIR ${MODULES_DIR}/libavpkt ${ROOT_DIR}/build/x64/modules/libavpkt)
	
	if(CMAKE_BUILD_TYPE MATCHES "Debug")
		set(DEPENDS 
			${BOOST_DIR}/libboost_thread-vc143-mt-sgd-x64-1_78.lib
			${BOOST_DIR}/libboost_chrono-vc143-mt-sgd-x64-1_78.lib
			${FFMPEG_DIR}/avcodec.lib 
			${FFMPEG_DIR}/avutil.lib 
			${FFMPEG_DIR}/swscale.lib 
			${AVPKT_DIR}/${CMAKE_BUILD_TYPE}/avpkt.lib)
	elseif(CMAKE_BUILD_TYPE MATCHES "Release")
		set(DEPENDS 
			${BOOST_DIR}/libboost_thread-vc143-mt-s-x64-1_78.lib
			${BOOST_DIR}/libboost_chrono-vc143-mt-s-x64-1_78.lib 
			${FFMPEG_DIR}/avcodec.lib 
			${FFMPEG_DIR}/avutil.lib 
			${FFMPEG_DIR}/swscale.lib 
			${AVPKT_DIR}/${CMAKE_BUILD_TYPE}/avpkt.lib)
	endif()
	
	add_definitions(-DUSE_MODULE_AV_PKT -D__STDC_CONSTANT_MACROS)
endif()

set(LIB_DIR ${MODULES_DIR}/libavcodec)
include_directories(${INCLUDE_DIR} ${BOOST_DIR} ${FFMPEG_DIR} ${AVPKT_DIR} ${LIB_DIR})
file(GLOB_RECURSE src "*.cpp")

add_library(${PROJECT_NAME} SHARED ${src})
target_link_libraries(${PROJECT_NAME} PRIVATE ${DEPENDS})