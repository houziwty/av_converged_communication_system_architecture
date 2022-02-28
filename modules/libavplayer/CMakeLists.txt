cmake_minimum_required(VERSION 3.5...3.16)

project(avplayer VERSION "${BUILD_VERSION_STRING}" LANGUAGES CXX)

set(
	AVPKT_ROOT_DIR 
	${ROOT_DIR}/modules/libavpkt)
set(
	AVPLAYER_ROOT_DIR 
	${ROOT_DIR}/modules/libavplayer)

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
	set(
		BOOST_ROOT_DIR 
		${3RDPART_DIR}/boost_1_78_0)
	set(
		BOOST_LINK_DIR 
		${BOOST_ROOT_DIR}/stage/lib)
	set(
		D3D_ROOT_DIR 
		${ROOT_DIR}/3rd/d3d/include)
	set(
		D3D_LINK_DIR 
		${ROOT_DIR}/3rd/d3d/lib)
	set(
		AVPKT_LINK_DIR 
		${ROOT_DIR}/build/x64/modules/libavpkt/${CMAKE_BUILD_TYPE})
	if(CMAKE_BUILD_TYPE MATCHES "Debug")
		set(
			LINK_LIBS 
			${BOOST_LINK_DIR}/libboost_thread-vc142-mt-sgd-x64-1_78.lib
			${BOOST_LINK_DIR}/libboost_chrono-vc142-mt-sgd-x64-1_78.lib 
			${AVPKT_LINK_DIR}/avpkt.lib 
			${D3D_LINK_DIR}/d3d9.lib 
			${D3D_LINK_DIR}/d3dx9.lib)
	elseif(CMAKE_BUILD_TYPE MATCHES "Release")
		set(
			LINK_LIBS 
			${BOOST_LINK_DIR}/libboost_thread-vc142-mt-s-x64-1_78.lib
			${BOOST_LINK_DIR}/libboost_chrono-vc142-mt-s-x64-1_78.lib 
			${AVPKT_LINK_DIR}/avpkt.lib 
			${D3D_LINK_DIR}/d3d9.lib 
			${D3D_LINK_DIR}/d3dx9.lib)
	endif()
	add_definitions(-DUSE_MODULE_AV_PKT)
endif()

include_directories(${INCLUDE_DIR} ${BOOST_ROOT_DIR} ${AVPKT_ROOT_DIR} ${AVPLAYER_ROOT_DIR})
file(GLOB_RECURSE main "*.cpp")

if(CMAKE_SYSTEM_NAME MATCHES "Windows")
	add_library(
		${PROJECT_NAME} 
		SHARED 
		${main})
	target_link_libraries(
		${PROJECT_NAME} 
		PRIVATE 
		${LINK_LIBS})
endif()
