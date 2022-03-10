cmake_minimum_required(VERSION 3.5...3.16)

project(avparser VERSION "${BUILD_VERSION_STRING}" LANGUAGES CXX)

set(
	AVPKT_ROOT_DIR 
	${ROOT_DIR}/modules/libavpkt)

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
	link_directories(${AVPKT_ROOT_DIR})
	set(
		LINK_LIBS 
		avpkt)
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
	set(
		BOOST_ROOT_DIR 
		${3RDPART_DIR}/boost_1_78_0)
	set(
		BOOST_LINK_DIR 
		${BOOST_ROOT_DIR}/stage/lib)
	set(
		AVPKT_LINK_DIR 
		${ROOT_DIR}/build/x64/modules/libavpkt/${CMAKE_BUILD_TYPE})
	if(CMAKE_BUILD_TYPE MATCHES "Debug")
		set(
			LINK_LIBS 
			${BOOST_LINK_DIR}/libboost_thread-vc143-mt-sgd-x64-1_78.lib 
			${BOOST_LINK_DIR}/libboost_chrono-vc143-mt-sgd-x64-1_78.lib 
			${AVPKT_LINK_DIR}/avpkt.lib)
	elseif(CMAKE_BUILD_TYPE MATCHES "Release")
		set(
			LINK_LIBS 
			${BOOST_LINK_DIR}/libboost_thread-vc143-mt-s-x64-1_78.lib 
			${BOOST_LINK_DIR}/libboost_chrono-vc143-mt-s-x64-1_78.lib 
			${AVPKT_LINK_DIR}/avpkt.lib)
	endif()
	add_definitions(-DUSE_MODULE_AV_PKT)
endif()

set(LIB_ROOT_DIR ${ROOT_DIR}/modules/libavparser)
include_directories(${INCLUDE_DIR} ${LIB_ROOT_DIR} ${AVPKT_ROOT_DIR} ${BOOST_ROOT_DIR})
file(GLOB_RECURSE main *.cpp)
file(GLOB_RECURSE 
	utils 
	"${SOURCE_DIR}/utils/memory/*.cpp")
file(GLOB_RECURSE 
	mpeg 
	"${SOURCE_DIR}/av/mpeg/*.c")
file(GLOB_RECURSE 
	sps 
	"${SOURCE_DIR}/av/sps/*.cpp")

add_library(
	${PROJECT_NAME} 
	SHARED 
	${main} 
    ${utils} 
	${mpeg} 
	${sps})
target_link_libraries(
	${PROJECT_NAME} 
	PRIVATE 
	${LINK_LIBS})