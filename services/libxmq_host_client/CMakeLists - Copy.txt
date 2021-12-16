cmake_minimum_required(VERSION 3.5...3.16)

project(xmq_host_client VERSION "${BUILD_VERSION_STRING}" LANGUAGES CXX)

include_directories(${INCLUDE_DIR})
link_directories(${LINK_DIR}/libxmq)

file(GLOB_RECURSE main "*.cpp")
file(GLOB_RECURSE 
	xmq 
	"${SOURCE_DIR}/network/xmq/worker_model.cpp")
file(GLOB_RECURSE 
	utils 
	"${SOURCE_DIR}/utils/time/*.cpp"
	"${SOURCE_DIR}/utils/thread/*.cpp"
	"${SOURCE_DIR}/utils/url/*.cpp")

add_library(
	${PROJECT_NAME} 
	SHARED 
	${main} 
	${xmq} 
	${utils})
target_link_libraries(
	${PROJECT_NAME} 
	PRIVATE 
	libboost_thread.a 
	xmq)