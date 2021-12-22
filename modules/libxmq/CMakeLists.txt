cmake_minimum_required(VERSION 3.5...3.16)

project(xmq VERSION "${BUILD_VERSION_STRING}" LANGUAGES CXX)

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
	set(
		ZMQ_LINK_LIBS 
		libzmq.a 
		gnutls 
		sodium 
		bsd)
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
	set(
		ZMQ_ROOT_DIR 
		${3RDPART_DIR}/libzmq-master)
	set(
		BOOST_ROOT_DIR 
		${3RDPART_DIR}/boost_1_78_0)
	set(
		ZMQ_LINK_DIR 
		${ZMQ_ROOT_DIR}/win32/lib/Release)
	set(
		BOOST_LINK_DIR 
		${BOOST_ROOT_DIR}/stage/lib)
	set(
		ZMQ_LINK_LIBS 
		${ZMQ_LINK_DIR}/libzmq-v142-mt-s-4_3_5.lib 
		${ZMQ_LINK_DIR}/libzmq-v142-mt-sgd-4_3_5.lib)
	set(
		BOOST_LINK_LIBS 
		${BOOST_LINK_DIR}/libboost_serialization-vc142-mt-s-x64-1_78.lib 
		${BOOST_LINK_DIR}/libboost_serialization-vc142-mt-sgd-x64-1_78.lib)
	set(
		WIN32_LINK_LIBS 
		ws2_32.lib 
		Iphlpapi.lib)
endif()

set(LIB_ROOT_DIR ${ROOT_DIR}/modules/libxmq)
include_directories(${INCLUDE_DIR} ${LIB_ROOT_DIR} ${ZMQ_ROOT_DIR}/include ${BOOST_ROOT_DIR})
add_definitions(-DZMQ_STATIC)
file(GLOB_RECURSE main "*.cpp")
file(GLOB_RECURSE utils "${SOURCE_DIR}/utils/hardware/*.cpp")

add_library(
	${PROJECT_NAME} 
	SHARED 
	${main} 
	${utils})
target_link_libraries(
	${PROJECT_NAME} 
	PRIVATE 
	${ZMQ_LINK_LIBS} 
	${BOOST_LINK_LIBS} 
	${WIN32_LINK_LIBS})