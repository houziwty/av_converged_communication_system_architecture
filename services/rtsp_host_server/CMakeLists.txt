cmake_minimum_required(VERSION 3.5...3.16)

project(rtsp_host_server VERSION "${BUILD_VERSION_STRING}" LANGUAGES CXX)

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
	set(FILELOG_DIR ${MODULES_DIR}/libfilelog)
	set(ASIO_DIR ${MODULES_DIR}/libasio)
	set(DEPENDS libboost_filesystem.a libboost_json.a pthread filelog asio)
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
	set(BOOST_DIR ${3RDPART_DIR}/boost_1_78_0 ${3RDPART_DIR}/boost_1_78_0/stage/lib)
	set(ASIO_DIR ${MODULES_DIR}/libasio ${ROOT_DIR}/build/x64/modules/libasio)
	set(FILELOG_DIR ${MODULES_DIR}/libfilelog ${ROOT_DIR}/build/x64/modules/libfilelog)
	
	if(CMAKE_BUILD_TYPE MATCHES "Debug")
		set(DEPENDS 
			${BOOST_DIR}/libboost_filesystem-vc143-mt-sgd-x64-1_78.lib 
			${BOOST_DIR}/libboost_json-vc143-mt-sgd-x64-1_78.lib 
			${BOOST_DIR}/libboost_atomic-vc143-mt-sgd-x64-1_78.lib 
			${BOOST_DIR}/libboost_thread-vc143-mt-sgd-x64-1_78.lib 
			${BOOST_DIR}/libboost_chrono-vc143-mt-sgd-x64-1_78.lib 
			${BOOST_DIR}/libboost_container-vc143-mt-sgd-x64-1_78.lib 
			${FILELOG_DIR}/${CMAKE_BUILD_TYPE}/filelog.lib 
			${ASIO_DIR}/${CMAKE_BUILD_TYPE}/asio.lib)
	elseif(CMAKE_BUILD_TYPE MATCHES "Release")
		set(DEPENDS 
			${BOOST_DIR}/libboost_filesystem-vc143-mt-s-x64-1_78.lib 
			${BOOST_DIR}/libboost_json-vc143-mt-s-x64-1_78.lib 
			${BOOST_DIR}/libboost_atomic-vc143-mt-s-x64-1_78.lib 
			${BOOST_DIR}/libboost_thread-vc143-mt-s-x64-1_78.lib 
			${BOOST_DIR}/libboost_chrono-vc143-mt-s-x64-1_78.lib 
			${BOOST_DIR}/libboost_container-vc143-mt-s-x64-1_78.lib
			${FILELOG_DIR}/${CMAKE_BUILD_TYPE}/filelog.lib 
			${ASIO_DIR}/${CMAKE_BUILD_TYPE}/asio.lib)
	endif()
	
	add_definitions(-DUSE_MODULE_FILE_LOG -DUSE_MODULE_NETWORK_ASIO)
endif()

include_directories(${INCLUDE_DIR} ${BOOST_DIR} ${ASIO_DIR} ${FILELOG_DIR})
file(GLOB_RECURSE src 
	"*.cpp" 
	"${SOURCE_DIR}/file/*.cpp" 
	"${SOURCE_DIR}/memory/*.cpp")

add_executable(${PROJECT_NAME} ${src})
target_link_libraries(${PROJECT_NAME} PRIVATE ${DEPENDS})