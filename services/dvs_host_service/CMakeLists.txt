cmake_minimum_required(VERSION 3.5...3.16)

project(dvs_host_service VERSION "${BUILD_VERSION_STRING}" LANGUAGES CXX)

include_directories(${INCLUDE_DIR})
link_directories(${LINK_DIR}/liblog)
link_directories(${LINK_DIR}/libxmq_host_client)
link_directories(${LINK_DIR}/libasio)
link_directories(${LINK_DIR}/libdvshost)

file(GLOB_RECURSE main "*.cpp")
file(GLOB_RECURSE 
	utils 
	"${SOURCE_DIR}/utils/commandline/*.cpp" 
	"${SOURCE_DIR}/utils/time/*.cpp"
	"${SOURCE_DIR}/utils/uuid/*.cpp")
file(GLOB_RECURSE 
	dvs 
	"${SOURCE_DIR}/dvs/*.cpp")

add_executable(
	${PROJECT_NAME} 
	${main} 
	${utils}
	${dvs})
target_link_libraries(
	${PROJECT_NAME} 
	PRIVATE 
	asio 
	log 
	libboost_program_options.a 
	libboost_thread.a 
	pthread 
	xmq_host_client 
	dvshost)