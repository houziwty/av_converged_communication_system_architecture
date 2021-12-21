cmake_minimum_required(VERSION 3.5...3.16)

project(avstream VERSION "${BUILD_VERSION_STRING}" LANGUAGES CXX)

if(CMAKE_SYSTEM_NAME MATCHES "Windows")
	set(
		BOOST_ROOT_DIR 
		${3RDPART_DIR}/boost_1_78_0)
	set(
		LINK_DIR 
		${BOOST_ROOT_DIR}/stage/lib)
	set(
		LINK_LIBS 
		${LINK_DIR}/libboost_thread-vc142-mt-gd-x64-1_78.lib
		${LINK_DIR}/libboost_chrono-vc142-mt-gd-x64-1_78.lib)
endif()

set(LIB_ROOT_DIR ${ROOT_DIR}/modules/libavstream)
include_directories(${INCLUDE_DIR} ${LIB_ROOT_DIR} ${BOOST_ROOT_DIR})
file(GLOB_RECURSE main "*.cpp")

add_library(
	${PROJECT_NAME} 
	SHARED 
	${main})