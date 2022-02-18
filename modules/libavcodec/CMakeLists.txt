cmake_minimum_required(VERSION 3.5...3.16)

project(avcodec VERSION "${BUILD_VERSION_STRING}" LANGUAGES CXX)

set(
	AVPKT_ROOT_DIR 
	${ROOT_DIR}/modules/libavpkt)

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,-Bsymbolic")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wl,-Bsymbolic")
	link_directories(${AVPKT_ROOT_DIR})
	set(
		LINK_LIBS 
		libavcodec.a 
		libavformat.a 
		libavutil.a 
		libswscale.a
		avpkt)
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
	set(
		BOOST_ROOT_DIR 
		${3RDPART_DIR}/boost_1_78_0)
	set(
		BOOST_LINK_DIR 
		${BOOST_ROOT_DIR}/stage/lib)
	set(
		FFMPEG_ROOT_DIR 
		${3RDPART_DIR}/ffmpeg/x64/ffmpeg-n5.0-latest-win64-gpl-shared-5.0/include)
	set(
		FFMPEG_LIK_DIR 
		${3RDPART_DIR}/ffmpeg/x64/ffmpeg-n5.0-latest-win64-gpl-shared-5.0/lib)
	set(
		AVPKT_LINK_DIR 
		${ROOT_DIR}/build/win32/modules/libavpkt/${CMAKE_BUILD_TYPE})
	if(CMAKE_BUILD_TYPE MATCHES "Debug")
		set(
			LINK_LIBS 
			${BOOST_LINK_DIR}/libboost_thread-vc142-mt-sgd-x64-1_78.lib
			${BOOST_LINK_DIR}/libboost_chrono-vc142-mt-sgd-x64-1_78.lib
			${FFMPEG_LIK_DIR}/avcodec.lib 
			${FFMPEG_LIK_DIR}/avutil.lib 
			${AVPKT_LINK_DIR}/avpkt.lib)
	elseif(CMAKE_BUILD_TYPE MATCHES "Release")
		set(
			LINK_LIBS 
			${BOOST_LINK_DIR}/libboost_thread-vc142-mt-s-x64-1_78.lib
			${BOOST_LINK_DIR}/libboost_chrono-vc142-mt-s-x64-1_78.lib 
			${FFMPEG_LIK_DIR}/avcodec.lib 
			${FFMPEG_LIK_DIR}/avutil.lib 
			${AVPKT_LINK_DIR}/avpkt.lib)
	endif()
	add_definitions(-DUSE_MODULE_AV_PKT)
endif()

set(LIB_ROOT_DIR ${ROOT_DIR}/modules/libavcodec)
include_directories(${INCLUDE_DIR} ${LIB_ROOT_DIR} ${BOOST_ROOT_DIR} ${AVPKT_ROOT_DIR} ${FFMPEG_ROOT_DIR})
file(GLOB_RECURSE main "*.cpp")

add_library(
	${PROJECT_NAME} 
	SHARED 
	${main})
target_link_libraries(
	${PROJECT_NAME} 
	PRIVATE 
	${LINK_LIBS})