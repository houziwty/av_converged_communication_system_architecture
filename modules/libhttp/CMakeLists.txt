cmake_minimum_required(VERSION 3.5...3.16)

project(http VERSION "${BUILD_VERSION_STRING}" LANGUAGES CXX)

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
	set(
		LINK_LIBS 
		asio)
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
			${LINK_DIR}/libboost_thread-vc142-mt-sgd-x64-1_78.lib
			${LINK_DIR}/libboost_chrono-vc142-mt-sgd-x64-1_78.lib)
	elseif(CMAKE_BUILD_TYPE MATCHES "Release")
		set(
			LINK_LIBS 
			${LINK_DIR}/libboost_thread-vc142-mt-s-x64-1_78.lib
			${LINK_DIR}/libboost_chrono-vc142-mt-s-x64-1_78.lib)
	endif()
endif()

set(ASIO_ROOT_DIR ${ROOT_DIR}/modules/libasio)
set(LIB_ROOT_DIR ${ROOT_DIR}/modules/libhttp)
include_directories(${INCLUDE_DIR} ${ASIO_ROOT_DIR} ${LIB_ROOT_DIR} ${BOOST_ROOT_DIR})
file(GLOB_RECURSE main *.cpp)
file(GLOB_RECURSE 
	utils 
	"${SOURCE_DIR}/utils/uuid/*.cpp")

add_library(
	${PROJECT_NAME} 
	SHARED 
	${main} 
	${utils})
target_link_libraries(
	${PROJECT_NAME} 
	PRIVATE 
	${LINK_LIBS})