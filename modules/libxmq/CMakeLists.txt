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
		${ZMQ_ROOT_DIR}/win32/lib/${CMAKE_BUILD_TYPE})
	set(
		BOOST_LINK_DIR 
		${BOOST_ROOT_DIR}/stage/lib)
	if(CMAKE_BUILD_TYPE MATCHES "Debug")
		set(
			LINK_LIBS 
			${ZMQ_LINK_DIR}/libzmq-v142-mt-sgd-4_3_5.lib
			${BOOST_LINK_DIR}/libboost_serialization-vc142-mt-sgd-x64-1_78.lib 
			ws2_32.lib 
			Iphlpapi.lib)
	elseif(CMAKE_BUILD_TYPE MATCHES "Release")
		set(
			LINK_LIBS 
			${ZMQ_LINK_DIR}/libzmq-v142-mt-s-4_3_5.lib
			${BOOST_LINK_DIR}/libboost_serialization-vc142-mt-s-x64-1_78.lib 
			ws2_32.lib 
			Iphlpapi.lib)
	endif()
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
	${LINK_LIBS})