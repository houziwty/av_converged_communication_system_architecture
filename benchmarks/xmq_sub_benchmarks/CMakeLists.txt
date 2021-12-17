cmake_minimum_required(VERSION 3.5...3.16)

project(xmq_sub_benchmarks VERSION "${BUILD_VERSION_STRING}" LANGUAGES CXX)

include_directories(${INCLUDE_DIR})
link_directories(${LINK_DIR}/libxmq)

file(GLOB_RECURSE main "*.cpp")
file(GLOB_RECURSE frameworks "${SOURCE_DIR}/network/xmq/sub.cpp" "${SOURCE_DIR}/utils/thread/*.cpp" "${SOURCE_DIR}/utils/time/*.cpp")

add_executable(
	${PROJECT_NAME} 
	${main} 
	${frameworks})
target_link_libraries(
	${PROJECT_NAME} 
	PRIVATE 
	xmq)