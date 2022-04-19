cmake_minimum_required(VERSION 3.5...3.16)

project(dvs_host_server VERSION "${BUILD_VERSION_STRING}" LANGUAGES CXX)

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
	set(FILELOG_DIR ${MODULES_DIR}/libfilelog)
	set(XMQ_DIR ${MODULES_DIR}/libxmq)
	set(ASIO_DIR ${MODULES_DIR}/libasio)
	set(DVS_DIR ${MODULES_DIR}/libdvs)
	set(AVPARSER_DIR ${MODULES_DIR}/libavparser)
	set(AVPKT_DIR ${MODULES_DIR}/libavpkt)
	set(DEPENDS libboost_program_options.a libboost_json.a pthread filelog xmq dvs asio avparser avpkt)
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
	set(BOOST_DIR ${3RDPART_DIR}/boost_1_78_0 ${3RDPART_DIR}/boost_1_78_0/stage/lib)
	set(FILELOG_DIR ${MODULES_DIR}/libfilelog ${ROOT_DIR}/build/x64/modules/libfilelog)
	set(XMQ_DIR ${MODULES_DIR}/libxmq ${ROOT_DIR}/build/x64/modules/libxmq)
	set(ASIO_DIR ${MODULES_DIR}/libasio ${ROOT_DIR}/build/x64/modules/libasio)
	set(DVS_DIR ${MODULES_DIR}/libdvs ${ROOT_DIR}/build/x64/modules/libdvs)
	set(AVPARSER_DIR ${MODULES_DIR}/libavparser ${ROOT_DIR}/build/x64/modules/libavparser)
	set(AVPKT_DIR  ${MODULES_DIR}/libavpkt ${ROOT_DIR}/build/x64/modules/libavpkt)
	
	if(CMAKE_BUILD_TYPE MATCHES "Debug")
		set(DEPENDS 
			${BOOST_DIR}/libboost_thread-vc143-mt-sgd-x64-1_78.lib 
			${BOOST_DIR}/libboost_chrono-vc143-mt-sgd-x64-1_78.lib 
			${BOOST_DIR}/libboost_program_options-vc143-mt-sgd-x64-1_78.lib 
			${BOOST_DIR}/libboost_serialization-vc143-mt-sgd-x64-1_78.lib 
			${BOOST_DIR}/libboost_atomic-vc143-mt-sgd-x64-1_78.lib 
			${BOOST_DIR}/libboost_json-vc143-mt-sgd-x64-1_78.lib 
			${BOOST_DIR}/libboost_container-vc143-mt-sgd-x64-1_78.lib 
			${FILELOG_DIR}/${CMAKE_BUILD_TYPE}/filelog.lib 
			${XMQ_DIR}/${CMAKE_BUILD_TYPE}/xmq.lib 
			${ASIO_DIR}/${CMAKE_BUILD_TYPE}/asio.lib 
			${AVPARSER_DIR}/${CMAKE_BUILD_TYPE}/avparser.lib 
			${AVPKT_DIR}/${CMAKE_BUILD_TYPE}/avpkt.lib 
			${DVS_DIR}/${CMAKE_BUILD_TYPE}/dvs.lib)
	elseif(CMAKE_BUILD_TYPE MATCHES "Release")
		set(DEPENDS 
			${BOOST_DIR}/libboost_thread-vc143-mt-s-x64-1_78.lib 
			${BOOST_DIR}/libboost_chrono-vc143-mt-s-x64-1_78.lib 
			${BOOST_DIR}/libboost_program_options-vc143-mt-s-x64-1_78.lib 
			${BOOST_DIR}/libboost_serialization-vc143-mt-s-x64-1_78.lib 
			${BOOST_DIR}/libboost_atomic-vc143-mt-s-x64-1_78.lib 
			${BOOST_DIR}/libboost_json-vc143-mt-s-x64-1_78.lib 
			${BOOST_DIR}/libboost_container-vc143-mt-s-x64-1_78.lib 
			${FILELOG_DIR}/${CMAKE_BUILD_TYPE}/filelog.lib 
			${XMQ_DIR}/${CMAKE_BUILD_TYPE}/xmq.lib 
			${ASIO_DIR}/${CMAKE_BUILD_TYPE}/asio.lib 
			${AVPARSER_DIR}/${CMAKE_BUILD_TYPE}/avparser.lib 
			${AVPKT_DIR}/${CMAKE_BUILD_TYPE}/avpkt.lib 
			${DVS_DIR}/${CMAKE_BUILD_TYPE}/dvs.lib)
	endif()
	
	add_definitions(-DUSE_MODULE_NETWORK_XMQ -DUSE_MODULE_NETWORK_ASIO -DUSE_MODULE_DEVICE_DVS -DUSE_MODULE_AV_PARSER -DUSE_MODULE_AV_PKT)
endif()

include_directories(${INCLUDE_DIR} ${BOOST_DIR} ${XMQ_DIR} ${ASIO_DIR} ${AVPARSER_DIR} ${AVPKT_DIR} ${DVS_DIR} ${FILELOG_DIR})
file(GLOB_RECURSE 
	src 
	"*.cpp"
	"${SOURCE_DIR}/commandline/*.cpp" 
	"${SOURCE_DIR}/time/*.cpp" 
	"${SOURCE_DIR}/url/*.cpp" 
	"${SOURCE_DIR}/memory/*.cpp")

add_executable(${PROJECT_NAME} ${src})
target_link_libraries(${PROJECT_NAME} PRIVATE ${DEPENDS})