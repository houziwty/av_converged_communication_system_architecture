cmake_minimum_required(VERSION 3.5...3.16)

project(http_flv_stream_server VERSION "${BUILD_VERSION_STRING}" LANGUAGES CXX)

include_directories(${INCLUDE_DIR})
link_directories(${LINK_DIR}/libasio)

file(GLOB_RECURSE main "*.cpp")

add_executable(
	${PROJECT_NAME} 
	${main})
target_link_libraries(
	${PROJECT_NAME} 
	PRIVATE 
	asio)