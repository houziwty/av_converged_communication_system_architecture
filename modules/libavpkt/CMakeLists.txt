cmake_minimum_required(VERSION 3.5...3.16)

project(avpkt VERSION "${BUILD_VERSION_STRING}" LANGUAGES CXX)

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
	set(BOOST_DIR ${3RDPART_DIR}/boost_1_78_0 ${3RDPART_DIR}/boost_1_78_0/stage/lib)
	
	if(CMAKE_BUILD_TYPE MATCHES "Debug")
	elseif(CMAKE_BUILD_TYPE MATCHES "Release")
	endif()
endif()

set(LIB_DIR ${MODULES_DIR}/libavpkt)
include_directories(${INCLUDE_DIR} ${BOOST_DIR} ${LIB_DIR})
file(GLOB_RECURSE 
	src 
	"*.cpp" 
	"${SOURCE_DIR}/memory/*.cpp")

add_library(${PROJECT_NAME} SHARED ${src})