cmake_minimum_required(VERSION 3.5...3.16)

project(filelog VERSION "${BUILD_VERSION_STRING}" LANGUAGES CXX)

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
	set(DEPENDS libglog.a libboost_filesystem.a pthread)
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
	set(BOOST_DIR ${3RDPART_DIR}/boost_1_78_0 ${3RDPART_DIR}/boost_1_78_0/stage/lib)
	set(GLOG_DIR ${3RDPART_DIR}/glog-master/build/x64)
	
	if(CMAKE_BUILD_TYPE MATCHES "Debug")
		set(DEPENDS 
			${GLOG_DIR}/${CMAKE_BUILD_TYPE}/glogd.lib 
			${BOOST_DIR}/libboost_filesystem-vc143-mt-sgd-x64-1_78.lib)
	elseif(CMAKE_BUILD_TYPE MATCHES "Release")
		set(DEPENDS 
			${GLOG_DIR}/${CMAKE_BUILD_TYPE}/glog.lib 
			${BOOST_DIR}/libboost_filesystem-vc143-mt-s-x64-1_78.lib)
	endif()
	
	add_definitions(-DGLOG_NO_ABBREVIATED_SEVERITIES)
endif()

include_directories(${INCLUDE_DIR} ${BOOST_DIR} ${GLOG_DIR})
file(GLOB_RECURSE src "*.cpp")

add_library(${PROJECT_NAME} SHARED ${src})
target_link_libraries(${PROJECT_NAME} PRIVATE ${DEPENDS})