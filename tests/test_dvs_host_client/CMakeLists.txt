cmake_minimum_required(VERSION 3.5...3.16)

project(test_dvs_host_client VERSION "${BUILD_VERSION_STRING}" LANGUAGES CXX)

include_directories(${INCLUDE_DIR})
link_directories(${LINK_DIR}/liblog)
link_directories(${LINK_DIR}/libxmq_host_client)

file(GLOB_RECURSE main "*.cpp")
file(GLOB_RECURSE 
	utils 
	"${SOURCE_DIR}/utils/commandline/*.cpp")

add_executable(
	${PROJECT_NAME} 
	${main} 
	${utils})
target_link_libraries(
	${PROJECT_NAME} 
	PRIVATE 
	log 
	libboost_program_options.a 
	xmq_host_client)