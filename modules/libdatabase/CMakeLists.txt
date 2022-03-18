cmake_minimum_required(VERSION 3.5...3.16)

project(database VERSION "${BUILD_VERSION_STRING}" LANGUAGES CXX)

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
	set(
		LINK_LIBS 
		libboost_thread.a 
		libredis++.a 
		libhiredis_static.a)
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
	set(
		BOOST_ROOT_DIR 
		${3RDPART_DIR}/boost_1_78_0)
	set(
		LINK_DIR 
		${BOOST_ROOT_DIR}/stage/lib)
	if(CMAKE_BUILD_TYPE MATCHES "Debug")
		set(
			LINK_LIBS 
			${LINK_DIR}/libboost_thread-vc143-mt-sgd-x64-1_78.lib
			${LINK_DIR}/libboost_chrono-vc143-mt-sgd-x64-1_78.lib)
	elseif(CMAKE_BUILD_TYPE MATCHES "Release")
		set(
			LINK_LIBS 
			${LINK_DIR}/libboost_thread-vc143-mt-s-x64-1_78.lib
			${LINK_DIR}/libboost_chrono-vc143-mt-s-x64-1_78.lib)
	endif()
endif()

set(LIB_ROOT_DIR ${ROOT_DIR}/modules/libdatabase)
include_directories(${INCLUDE_DIR} ${LIB_ROOT_DIR} ${BOOST_ROOT_DIR})
file(GLOB_RECURSE main *.cpp)

add_library(
	${PROJECT_NAME} 
	SHARED 
	${main})
target_link_libraries(
	${PROJECT_NAME} 
	PRIVATE 
	${LINK_LIBS})
target_compile_features(
	${PROJECT_NAME} 
	PRIVATE 
	cxx_std_17)