cmake_minimum_required(VERSION 3.5...3.16)

project(xms_host_client VERSION "${BUILD_VERSION_STRING}" LANGUAGES CXX)

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
		ASIO_LINK_DIR 
		${ROOT_DIR}/win32/modules/libasio/${CMAKE_BUILD_TYPE})
	set(
		LOG_LINK_DIR 
		${ROOT_DIR}/win32/modules/liblog/${CMAKE_BUILD_TYPE})
	set(
		LINK_LIBS 
		${BOOST_LINK_DIR}/libboost_thread-vc142-mt-gd-x64-1_78.lib 
		${BOOST_LINK_DIR}/libboost_chrono-vc142-mt-gd-x64-1_78.lib 
		${BOOST_LINK_DIR}/libboost_serialization-vc142-mt-gd-x64-1_78.lib 
		${ASIO_LINK_DIR}/asio.lib 
		${LOG_LINK_DIR}/log.lib)
	add_definitions(-DUSE_MODULE_FILE_LOG -DUSE_MODULE_NETWORK_XMQ)
endif()

set(SERVICE_ROOT_DIR ${ROOT_DIR}/service/libxms_host_client)
include_directories(${INCLUDE_DIR} ${SERVICE_ROOT_DIR} ${BOOST_ROOT_DIR} ${ASIO_ROOT_DIR} ${LOG_ROOT_DIR})
file(GLOB_RECURSE main "*.cpp")
file(GLOB_RECURSE 
	utils 
	"${SOURCE_DIR}/utils/uuid/*.cpp")

add_library(
	${PROJECT_NAME} 
	SHARED 
	${main} 
	${utils})
target_link_libraries(
	${PROJECT_NAME} 
	PRIVATE
	${LINK_LIBS})