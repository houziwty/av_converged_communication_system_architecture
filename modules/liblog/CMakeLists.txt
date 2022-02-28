cmake_minimum_required(VERSION 3.5...3.16)

project(log VERSION "${BUILD_VERSION_STRING}" LANGUAGES CXX)

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
	link_directories(${ROOT_DIR}/3rd/glog-master/build)
	set(
		LINK_LIBS 
		libglog.a 
		pthread)
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
	set(
		GLOG_ROOT_DIR 
		${3RDPART_DIR}/glog-master/build/x64/)
	if(CMAKE_BUILD_TYPE MATCHES "Debug")
		set(
			LINK_LIBS 
			${3RDPART_DIR}/glog-master/build/x64/${CMAKE_BUILD_TYPE}/glogd.lib)
	elseif(CMAKE_BUILD_TYPE MATCHES "Release")
		set(
			LINK_LIBS 
			${3RDPART_DIR}/glog-master/build/x64/${CMAKE_BUILD_TYPE}/glog.lib)
	endif()
	add_definitions(-DGLOG_NO_ABBREVIATED_SEVERITIES)
endif()

include_directories(${INCLUDE_DIR} ${GLOG_ROOT_DIR})
file(GLOB_RECURSE main "*.cpp")

add_library(
	${PROJECT_NAME} 
	SHARED 
	${main})
target_link_libraries(
	${PROJECT_NAME} 
	PRIVATE 
	${LINK_LIBS})