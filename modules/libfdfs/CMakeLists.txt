cmake_minimum_required(VERSION 3.5...3.16)

project(fdfs VERSION "${BUILD_VERSION_STRING}" LANGUAGES CXX)

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
	set(DEPENDS 
		${3RDPART_DIR}/fastdfs-master/client/libfdfsclient.a 
		${3RDPART_DIR}/libfastcommon-master/src/libfastcommon.a
		dl)
		
	include_directories(${INCLUDE_DIR} ${BOOST_DIR} ${FDFS_DIR})
	file(GLOB_RECURSE src "*.cpp")

	add_library(${PROJECT_NAME} SHARED ${src})
	target_link_libraries(${PROJECT_NAME} PRIVATE ${DEPENDS})
endif()