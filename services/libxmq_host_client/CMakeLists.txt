cmake_minimum_required(VERSION 3.5...3.16)

project(xmq_host_client VERSION "${BUILD_VERSION_STRING}" LANGUAGES CXX)

set(
	XMQ_ROOT_DIR 
	${ROOT_DIR}/modules/libxmq)
set(
	LOG_ROOT_DIR 
	${ROOT_DIR}/modules/liblog)
if(CMAKE_SYSTEM_NAME MATCHES "Linux")
	link_directories(${XMQ_ROOT_DIR} ${LOG_ROOT_DIR})
	set(
		LINK_LIBS 
		xmq 
		log 
		libboost_thread.a)
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
	set(
		BOOST_ROOT_DIR 
		${3RDPART_DIR}/boost_1_78_0)
	set(
		BOOST_LINK_DIR 
		${BOOST_ROOT_DIR}/stage/lib)
	set(
		XMQ_LINK_DIR 
		${ROOT_DIR}/win32/modules/libxmq/${CMAKE_BUILD_TYPE})
	set(
		LOG_LINK_DIR 
		${ROOT_DIR}/win32/modules/liblog/${CMAKE_BUILD_TYPE})
	if(CMAKE_BUILD_TYPE MATCHES "Debug")
		set(
			LINK_LIBS 
			${BOOST_LINK_DIR}/libboost_thread-vc142-mt-sgd-x64-1_78.lib 
			${BOOST_LINK_DIR}/libboost_chrono-vc142-mt-sgd-x64-1_78.lib 
			${BOOST_LINK_DIR}/libboost_serialization-vc142-mt-sgd-x64-1_78.lib 
			${XMQ_LINK_DIR}/xmq.lib 
			${LOG_LINK_DIR}/log.lib)
	elseif(CMAKE_BUILD_TYPE MATCHES "Release")
		set(
			LINK_LIBS 
			${BOOST_LINK_DIR}/libboost_thread-vc142-mt-s-x64-1_78.lib 
			${BOOST_LINK_DIR}/libboost_chrono-vc142-mt-s-x64-1_78.lib 
			${BOOST_LINK_DIR}/libboost_serialization-vc142-mt-s-x64-1_78.lib 
			${XMQ_LINK_DIR}/xmq.lib 
			${LOG_LINK_DIR}/log.lib)
	endif()
	add_definitions(-DUSE_MODULE_FILE_LOG -DUSE_MODULE_NETWORK_XMQ)
endif()

set(SERVICE_ROOT_DIR ${ROOT_DIR}/service/libxmq_host_client)
include_directories(${INCLUDE_DIR} ${SERVICE_ROOT_DIR} ${BOOST_ROOT_DIR} ${XMQ_ROOT_DIR} ${LOG_ROOT_DIR})
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
	${LINK_LIBS})