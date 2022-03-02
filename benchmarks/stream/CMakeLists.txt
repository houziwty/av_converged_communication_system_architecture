cmake_minimum_required(VERSION 3.5...3.16)

project(stream VERSION "${BUILD_VERSION_STRING}" LANGUAGES CXX)

set(
	ASIO_ROOT_DIR 
	${ROOT_DIR}/modules/libasio)
set(
	AVPARSER_ROOT_DIR 
	${ROOT_DIR}/modules/libavparser)
set(
	AVPKT_ROOT_DIR 
	${ROOT_DIR}/modules/libavpkt)
set(
	AVCODEC_ROOT_DIR 
	${ROOT_DIR}/modules/libavcodec)
set(
	AVPLAYER_ROOT_DIR 
	${ROOT_DIR}/modules/libavplayer)
set(
	AV_ROOT_DIR 
	${ROOT_DIR}/modules/libav)

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
	set(
		LINK_LIBS 
		asio 
		avparser 
		avpkt 
		avcodec 
		av)
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
endif()

set(LIB_ROOT_DIR ${ROOT_DIR}/benchmarks/stream)
include_directories(
	${INCLUDE_DIR} 
	${LIB_ROOT_DIR} 
	${ASIO_ROOT_DIR} 
	${AVPARSER_ROOT_DIR} 
	${AVPKT_ROOT_DIR} 
	${AVCODEC_ROOT_DIR} 
	${AVPLAYER_ROOT_DIR} 
	${AV_ROOT_DIR})
file(GLOB_RECURSE main "*.cpp")

add_executable(
	${PROJECT_NAME} 
	${main})
target_link_libraries(
	${PROJECT_NAME} 
	PRIVATE 
	${LINK_LIBS})
