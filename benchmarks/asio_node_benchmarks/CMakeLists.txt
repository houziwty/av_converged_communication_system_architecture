cmake_minimum_required(VERSION 3.5...3.16)

project(asio_node_benchmarks VERSION "${BUILD_VERSION_STRING}" LANGUAGES CXX)

set(
	ASIO_ROOT_DIR 
	${ROOT_DIR}/modules/libasio)
set(
	LOG_ROOT_DIR 
	${ROOT_DIR}/modules/liblog)
	
if(CMAKE_SYSTEM_NAME MATCHES "Linux")
	link_directories(${ASIO_ROOT_DIR} ${LOG_ROOT_DIR})
	set(
		LINK_LIBS 
		asio 
		log)
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
	set(
		BOOST_ROOT_DIR 
		${3RDPART_DIR}/boost_1_78_0)
	set(
		BOOST_LINK_DIR 
		${BOOST_ROOT_DIR}/stage/lib)
	set(
		ASIO_LINK_DIR 
		${ROOT_DIR}/win32/modules/libasio/${CMAKE_BUILD_TYPE})
	set(
		LOG_LINK_DIR 
		${ROOT_DIR}/win32/modules/liblog/${CMAKE_BUILD_TYPE})
	if(CMAKE_BUILD_TYPE MATCHES "Debug")
		set(
			LINK_LIBS 
			${BOOST_LINK_DIR}/libboost_thread-vc142-mt-sgd-x64-1_78.lib 
			${BOOST_LINK_DIR}/libboost_chrono-vc142-mt-sgd-x64-1_78.lib 
			${BOOST_LINK_DIR}/libboost_program_options-vc142-mt-sgd-x64-1_78.lib	
			${BOOST_LINK_DIR}/libboost_serialization-vc142-mt-sgd-x64-1_78.lib 		
			${XMQ_LINK_DIR}/asio.lib 
			${LOG_LINK_DIR}/log.lib)
	elseif(CMAKE_BUILD_TYPE MATCHES "Release")
		set(
			LINK_LIBS 
			${BOOST_LINK_DIR}/libboost_thread-vc142-mt-s-x64-1_78.lib 
			${BOOST_LINK_DIR}/libboost_chrono-vc142-mt-s-x64-1_78.lib 
			${BOOST_LINK_DIR}/libboost_program_options-vc142-mt-s-x64-1_78.lib	
			${BOOST_LINK_DIR}/libboost_serialization-vc142-mt-s-x64-1_78.lib 		
			${XMQ_LINK_DIR}/asio.lib 
			${LOG_LINK_DIR}/log.lib)
	endif()
	add_definitions(-DUSE_MODULE_FILE_LOG -DUSE_MODULE_NETWORK_ASIO)
endif()

set(TEST_ASIO_NODE_DIR ${ROOT_DIR}/benchmarks/asio_node_benchmarks)
include_directories(
	${INCLUDE_DIR} 
	${TEST_ASIO_NODE_DIR} 
	${BOOST_ROOT_DIR} 
	${ASIO_ROOT_DIR} 
	${LOG_ROOT_DIR})
file(GLOB_RECURSE main "*.cpp")

add_executable(
	${PROJECT_NAME} 
	${main})
target_link_libraries(
	${PROJECT_NAME} 
	PRIVATE 
	${LINK_LIBS})