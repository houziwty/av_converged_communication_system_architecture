cmake_minimum_required(VERSION 3.5...3.16)

project(xmq VERSION "${BUILD_VERSION_STRING}" LANGUAGES CXX)

include_directories(${INCLUDE_DIR})

file(GLOB_RECURSE main "*.cpp")
file(GLOB_RECURSE utils "${SOURCE_DIR}/utils/hardware/*.cpp")

add_library(
	${PROJECT_NAME} 
	SHARED 
	${main} 
	${utils})
target_link_libraries(
	${PROJECT_NAME} 
	PRIVATE 
	libzmq.a 
	gnutls 
	sodium 
	bsd)