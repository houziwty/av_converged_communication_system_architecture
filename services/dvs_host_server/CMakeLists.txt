cmake_minimum_required(VERSION 3.5...3.16)

project(dvs_host_server VERSION "${BUILD_VERSION_STRING}" LANGUAGES CXX)

set(
	XMQ_ROOT_DIR 
	${ROOT_DIR}/modules/libxmq)
set(
	LOG_ROOT_DIR 
	${ROOT_DIR}/modules/liblog)
set(
	ASIO_ROOT_DIR 
	${ROOT_DIR}/modules/libasio)
set(
	DVS_ROOT_DIR 
	${ROOT_DIR}/modules/libdvs)
set(
	AVPARSER_ROOT_DIR 
	${ROOT_DIR}/modules/libavparser)
set(
	AVPKT_ROOT_DIR 
	${ROOT_DIR}/modules/libavpkt)
if(CMAKE_SYSTEM_NAME MATCHES "Linux")
	link_directories(${XMQ_ROOT_DIR} ${ASIO_ROOT_DIR} ${LOG_ROOT_DIR} ${DVS_ROOT_DIR} ${AVPARSER_ROOT_DIR} ${AVPKT_ROOT_DIR})
	set(
		LINK_LIBS 
		libboost_program_options.a 
		pthread 
		dvs 
		asio 
		avparser 
		avpkt 
		xmq 
		log)
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
	set(
		BOOST_ROOT_DIR 
		${3RDPART_DIR}/boost_1_78_0)
	set(
		BOOST_LINK_DIR 
		${BOOST_ROOT_DIR}/stage/lib)
	set(
		XMQ_LINK_DIR 
		${ROOT_DIR}/build/x64/modules/libxmq/${CMAKE_BUILD_TYPE})
	set(
		LOG_LINK_DIR 
		${ROOT_DIR}/build/x64/modules/liblog/${CMAKE_BUILD_TYPE})
	set(
		ASIO_LINK_DIR 
		${ROOT_DIR}/build/x64/modules/libasio/${CMAKE_BUILD_TYPE})
	set(
		DVS_LINK_DIR 
		${ROOT_DIR}/build/x64/modules/libdvs/${CMAKE_BUILD_TYPE})
	set(
		AVPARSER_LINK_DIR 
		${ROOT_DIR}/build/x64/modules/libavparser/${CMAKE_BUILD_TYPE})
	set(
		AVPKT_LINK_DIR 
		${ROOT_DIR}/build/x64/modules/libavpkt/${CMAKE_BUILD_TYPE})
	if(CMAKE_BUILD_TYPE MATCHES "Debug")
		set(
			LINK_LIBS 
			${BOOST_LINK_DIR}/libboost_thread-vc142-mt-sgd-x64-1_78.lib 
			${BOOST_LINK_DIR}/libboost_chrono-vc142-mt-sgd-x64-1_78.lib 
			${BOOST_LINK_DIR}/libboost_program_options-vc142-mt-sgd-x64-1_78.lib	
			${BOOST_LINK_DIR}/libboost_serialization-vc142-mt-sgd-x64-1_78.lib 		
			${XMQ_LINK_DIR}/xmq.lib 
			${LOG_LINK_DIR}/log.lib 
			${ASIO_LINK_DIR}/asio.lib 
			${AVPARSER_LINK_DIR}/avparser.lib 
			${AVPKT_LINK_DIR}/avpkt.lib 
			${DVS_LINK_DIR}/dvs.lib)
	elseif(CMAKE_BUILD_TYPE MATCHES "Release")
		set(
			LINK_LIBS 
			${BOOST_LINK_DIR}/libboost_thread-vc142-mt-s-x64-1_78.lib 
			${BOOST_LINK_DIR}/libboost_chrono-vc142-mt-s-x64-1_78.lib 
			${BOOST_LINK_DIR}/libboost_program_options-vc142-mt-s-x64-1_78.lib	
			${BOOST_LINK_DIR}/libboost_serialization-vc142-mt-s-x64-1_78.lib 		
			${XMQ_LINK_DIR}/xmq.lib 
			${LOG_LINK_DIR}/log.lib 
			${ASIO_LINK_DIR}/asio.lib 
			${AVPARSER_LINK_DIR}/avparser.lib 
			${AVPKT_LINK_DIR}/avpkt.lib 
			${DVS_LINK_DIR}/dvs.lib)
	endif()
	add_definitions(-DUSE_MODULE_FILE_LOG -DUSE_MODULE_NETWORK_XMQ -DUSE_MODULE_NETWORK_ASIO -DUSE_MODULE_DEVICE_DVS -DUSE_MODULE_AV_PARSER -DUSE_MODULE_AV_PKT)
endif()

set(SERVER_ROOT_DIR ${ROOT_DIR}/service/dvs_host_server)
include_directories(
	${INCLUDE_DIR} 
	${SERVER_ROOT_DIR} 
	${BOOST_ROOT_DIR} 
	${XMQ_ROOT_DIR} 
	${LOG_ROOT_DIR} 
	${ASIO_ROOT_DIR} 
	${AVPARSER_ROOT_DIR} 
	${AVPKT_ROOT_DIR} 
	${DVS_ROOT_DIR})
file(GLOB_RECURSE main "*.cpp")
file(GLOB_RECURSE 
	utils 
	"${SOURCE_DIR}/utils/commandline/*.cpp" 
	"${SOURCE_DIR}/utils/time/*.cpp" 
	"${SOURCE_DIR}/utils/url/*.cpp" 
	"${SOURCE_DIR}/utils/memory/*.cpp")

add_executable(
	${PROJECT_NAME} 
	${main} 
	${utils})
target_link_libraries(
	${PROJECT_NAME} 
	PRIVATE 
	${LINK_LIBS})