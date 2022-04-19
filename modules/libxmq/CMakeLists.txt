cmake_minimum_required(VERSION 3.5...3.16)

project(xmq VERSION "${BUILD_VERSION_STRING}" LANGUAGES CXX)

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
	set(DEPENDS libboost_thread.a libzmq.a gnutls sodium bsd)
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
	set(BOOST_DIR ${3RDPART_DIR}/boost_1_78_0 ${3RDPART_DIR}/boost_1_78_0/stage/lib)
	set(ZMQ_DIR ${3RDPART_DIR}/libzmq-master/include ${3RDPART_DIR}/libzmq-master/build/x64/lib)
	
	if(CMAKE_BUILD_TYPE MATCHES "Debug")
		set(DEPENDS 
			${ZMQ_DIR}/${CMAKE_BUILD_TYPE}/libzmq-v142-mt-sgd-4_3_5.lib
			${BOOST_DIR}/libboost_serialization-vc143-mt-sgd-x64-1_78.lib 
			${BOOST_DIR}/libboost_thread-vc143-mt-sgd-x64-1_78.lib 
			${BOOST_DIR}/libboost_chrono-vc143-mt-sgd-x64-1_78.lib 
			ws2_32.lib 
			Iphlpapi.lib)
	elseif(CMAKE_BUILD_TYPE MATCHES "Release")
		set(DEPENDS 
			${ZMQ_DIR}/${CMAKE_BUILD_TYPE}/libzmq-v142-mt-s-4_3_5.lib
			${BOOST_DIR}/libboost_serialization-vc143-mt-s-x64-1_78.lib 
			${BOOST_DIR}/libboost_thread-vc143-mt-s-x64-1_78.lib 
			${BOOST_DIR}/libboost_chrono-vc143-mt-s-x64-1_78.lib 
			ws2_32.lib 
			Iphlpapi.lib)
	endif()
	
	add_definitions(-DZMQ_STATIC)
endif()

set(LIB_DIR ${MODULES_DIR}/libxmq)
include_directories(${INCLUDE_DIR} ${BOOST_DIR} ${ZMQ_DIR} ${LIB_DIR})
file(GLOB_RECURSE src 
	"*.cpp" 
	"${SOURCE_DIR}/hardware/*.cpp" 
	"${SOURCE_DIR}/url/*.cpp" 
	"${SOURCE_DIR}/time/*.cpp"
	"${SOURCE_DIR}/thread/*.cpp" 
	"${SOURCE_DIR}/memory/*.cpp")

add_library(${PROJECT_NAME} SHARED ${src})
target_link_libraries(${PROJECT_NAME} PRIVATE ${DEPENDS})