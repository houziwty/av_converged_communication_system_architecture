cmake_minimum_required(VERSION 3.5...3.16)

project(xmq_host_service VERSION "${BUILD_VERSION_STRING}" LANGUAGES CXX)

file(GLOB_RECURSE main "*.cpp")
file(GLOB_RECURSE xmq "${SOURCE_DIR}/network/xmq/switcher_pub_model.cpp")
file(GLOB_RECURSE 
	utils 
	"${SOURCE_DIR}/utils/url/*.cpp" 
	"${SOURCE_DIR}/utils/thread/*.cpp" 
	"${SOURCE_DIR}/utils/time/*.cpp" 
	"${SOURCE_DIR}/utils/commandline/*.cpp")

include_directories(${INCLUDE_DIR})
link_directories(${LINK_DIR}/liblog)
link_directories(${LINK_DIR}/libxmq)

add_executable(
	${PROJECT_NAME} 
	${main} 
	${xmq} 
	${utils})
target_link_libraries(
	${PROJECT_NAME} 
	PRIVATE 
	xmq log 
	libboost_program_options.a 
	libboost_thread.a 
	pthread)