cmake_minimum_required(VERSION 3.5...3.16)

project(asio VERSION "${BUILD_VERSION_STRING}" LANGUAGES CXX)

include_directories(${INCLUDE_DIR})

file(GLOB_RECURSE main *.cpp)

add_library(
	${PROJECT_NAME} 
	SHARED 
	${main})
target_link_libraries(
	${PROJECT_NAME} 
	PUBLIC 
	pthread)