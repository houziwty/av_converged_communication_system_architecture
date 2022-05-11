cmake_minimum_required(VERSION 3.5...3.16)

project(asio VERSION "${BUILD_VERSION_STRING}" LANGUAGES CXX)

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
	set(DEPENDS libboost_thread.a pthread)
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
	set(BOOST_DIR ${3RDPART_DIR}/boost_1_78_0 ${3RDPART_DIR}/boost_1_78_0/stage/lib)
	
	if(CMAKE_BUILD_TYPE MATCHES "Debug")
		set(DEPENDS 
			${BOOST_DIR}/libboost_thread-vc143-mt-sgd-x64-1_78.lib
			${BOOST_DIR}/libboost_chrono-vc143-mt-sgd-x64-1_78.lib)
	elseif(CMAKE_BUILD_TYPE MATCHES "Release")
		set(DEPENDS 
			${BOOST_DIR}/libboost_thread-vc143-mt-s-x64-1_78.lib
			${BOOST_DIR}/libboost_chrono-vc143-mt-s-x64-1_78.lib)
	endif()
endif()

set(LIB_DIR ${MODULES_DIR}/libasio)
include_directories(${INCLUDE_DIR} ${BOOST_DIR} ${LIB_DIR})
file(GLOB_RECURSE 
	src 
	*.cpp 
	"${SOURCE_DIR}/time/xtime.cpp")

add_library(${PROJECT_NAME} SHARED ${src})
target_link_libraries(${PROJECT_NAME} PRIVATE ${DEPENDS})