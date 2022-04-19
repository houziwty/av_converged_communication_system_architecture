cmake_minimum_required(VERSION 3.5...3.16)

project(avparser VERSION "${BUILD_VERSION_STRING}" LANGUAGES CXX)

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
	set(AVPKT_DIR ${MODULES_DIR}/libavpkt)
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
	set(BOOST_DIR ${3RDPART_DIR}/boost_1_78_0 ${3RDPART_DIR}/boost_1_78_0/stage/lib)
	set(AVPKT_DIR ${MODULES_DIR}/libavpkt ${ROOT_DIR}/build/x64/modules/libavpkt)
	
	if(CMAKE_BUILD_TYPE MATCHES "Debug")
		set(DEPENDS 
			${BOOST_DIR}/libboost_thread-vc143-mt-sgd-x64-1_78.lib 
			${BOOST_DIR}/libboost_chrono-vc143-mt-sgd-x64-1_78.lib 
			${AVPKT_DIR}/${CMAKE_BUILD_TYPE}/avpkt.lib)
	elseif(CMAKE_BUILD_TYPE MATCHES "Release")
		set(DEPENDS 
			${BOOST_DIR}/libboost_thread-vc143-mt-s-x64-1_78.lib 
			${BOOST_DIR}/libboost_chrono-vc143-mt-s-x64-1_78.lib 
			${AVPKT_DIR}/${CMAKE_BUILD_TYPE}/avpkt.lib)
	endif()
	
	add_definitions(-DUSE_MODULE_AV_PKT)
endif()

set(LIB_DIR ${MODULES_DIR}/libavparser)
include_directories(${INCLUDE_DIR} ${BOOST_DIR} ${AVPKT_DIR} ${LIB_DIR})
file(GLOB_RECURSE src 
	*.cpp 
	"${SOURCE_DIR}/memory/*.cpp" 
	"${SOURCE_DIR}/av/sps/*.cpp" 
	"${SOURCE_DIR}/av/mpeg/*.c")

add_library(${PROJECT_NAME} SHARED ${src})
target_link_libraries(${PROJECT_NAME} PRIVATE ${DEPENDS})