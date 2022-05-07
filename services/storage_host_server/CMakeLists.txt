cmake_minimum_required(VERSION 3.5...3.16)

project(storage_host_server VERSION "${BUILD_VERSION_STRING}" LANGUAGES CXX)

if(CMAKE_SYSTEM_NAME MATCHES "Linux")	
	set(FILELOG_DIR ${MODULES_DIR}/libfilelog)
	set(XMQ_DIR ${MODULES_DIR}/libxmq)
	set(ASIO_DIR ${MODULES_DIR}/libasio)
	set(FDFS_DIR ${MODULES_DIR}/libfdfs)
	set(AVPKT_DIR ${MODULES_DIR}/libavpkt)
	set(AVPARSER_DIR ${MODULES_DIR}/libavparser)
	set(DEPENDS libboost_program_options.a libboost_filesystem.a libboost_json.a pthread xmq asio filelog fdfs avpkt avparser)
	
	include_directories(${INCLUDE_DIR} ${BOOST_DIR} ${XMQ_DIR} ${ASIO_DIR} ${FILELOG_DIR} ${FDFS_DIR} ${AVPKT_DIR} ${AVPARSER_DIR})
	file(GLOB_RECURSE src 
		"*.cpp" 
		"${SOURCE_DIR}/commandline/*.cpp" 
		"${SOURCE_DIR}/url/*.cpp" 
		"${SOURCE_DIR}/memory/*.cpp")

	add_executable(${PROJECT_NAME} ${src})
	target_link_libraries(${PROJECT_NAME} PRIVATE ${DEPENDS})
endif()