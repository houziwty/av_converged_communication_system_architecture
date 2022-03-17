cmake_minimum_required(VERSION 3.5...3.16)

project(log VERSION "${BUILD_VERSION_STRING}" LANGUAGES CXX)

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
	link_directories(${ROOT_DIR}/3rd/glog-master/build)
	set(
		LINK_LIBS 
		libglog.a 
		libboost_filesystem.a )
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
	set(
		BOOST_ROOT_DIR 
		${3RDPART_DIR}/boost_1_78_0)
	set(
		BOOST_LINK_DIR 
		${BOOST_ROOT_DIR}/stage/lib)
	set(
		GLOG_ROOT_DIR 
		${3RDPART_DIR}/glog-master/build/x64/)
	if(CMAKE_BUILD_TYPE MATCHES "Debug")
		set(
			LINK_LIBS 
			${GLOG_ROOT_DIR}/${CMAKE_BUILD_TYPE}/glogd.lib 
			${BOOST_LINK_DIR}/libboost_filesystem-vc143-mt-sgd-x64-1_78.lib)
	elseif(CMAKE_BUILD_TYPE MATCHES "Release")
		set(
			LINK_LIBS 
			${GLOG_ROOT_DIR}/${CMAKE_BUILD_TYPE}/glog.lib 
			${BOOST_LINK_DIR}/libboost_filesystem-vc143-mt-s-x64-1_78.lib)
	endif()
	add_definitions(-DGLOG_NO_ABBREVIATED_SEVERITIES)
endif()

include_directories(${INCLUDE_DIR} ${BOOST_ROOT_DIR} ${GLOG_ROOT_DIR})
file(GLOB_RECURSE main "*.cpp")

add_library(
	${PROJECT_NAME} 
	SHARED 
	${main})
target_link_libraries(
	${PROJECT_NAME} 
	PRIVATE 
	${LINK_LIBS})