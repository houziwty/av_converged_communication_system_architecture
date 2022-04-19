cmake_minimum_required(VERSION 3.5...3.16)

project(avplayer VERSION "${BUILD_VERSION_STRING}" LANGUAGES CXX)

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
	set(BOOST_DIR ${3RDPART_DIR}/boost_1_78_0 ${3RDPART_DIR}/boost_1_78_0/stage/lib)
	set(D3D_DIR ${3RDPART_DIR}/d3d/include ${3RDPART_DIR}/d3d/lib)
	set(AVPKT_DIR ${MODULES_DIR}/libavpkt ${ROOT_DIR}/build/x64/modules/libavpkt)
	
	if(CMAKE_BUILD_TYPE MATCHES "Debug")
		set(DEPENDS 
			${BOOST_DIR}/libboost_thread-vc143-mt-sgd-x64-1_78.lib
			${BOOST_DIR}/libboost_chrono-vc143-mt-sgd-x64-1_78.lib 
			${D3D_DIR}/d3d9.lib 
			${D3D_DIR}/d3dx9.lib 
			${AVPKT_DIR}/${CMAKE_BUILD_TYPE}/avpkt.lib)
	elseif(CMAKE_BUILD_TYPE MATCHES "Release")
		set(
			LINK_LIBS 
			${BOOST_DIR}/libboost_thread-vc143-mt-s-x64-1_78.lib
			${BOOST_DIR}/libboost_chrono-vc143-mt-s-x64-1_78.lib 
			${D3D_DIR}/d3d9.lib 
			${D3D_DIR}/d3dx9.lib 
			${AVPKT_DIR}/${CMAKE_BUILD_TYPE}/avpkt.lib)
	endif()
	
	add_definitions(-DUSE_MODULE_AV_PKT)
endif()

set(LIB_DIR ${MODULES_DIR}/libavplayer)
include_directories(${INCLUDE_DIR} ${BOOST_DIR} ${D3D_DIR} ${AVPKT_DIR} ${LIB_DIR})
file(GLOB_RECURSE src "*.cpp")

if(CMAKE_SYSTEM_NAME MATCHES "Windows")
	add_library(${PROJECT_NAME} SHARED ${src})
	target_link_libraries(${PROJECT_NAME} PRIVATE ${DEPENDS})
endif()
