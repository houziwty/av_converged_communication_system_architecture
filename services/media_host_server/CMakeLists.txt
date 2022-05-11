cmake_minimum_required(VERSION 3.5...3.16)

project(media_host_server VERSION "${BUILD_VERSION_STRING}" LANGUAGES CXX)

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
	set(FILELOG_DIR ${MODULES_DIR}/libfilelog)
	set(ASIO_DIR ${MODULES_DIR}/libasio)
	set(DEPENDS filelog asio)
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
	set(BOOST_DIR ${3RDPART_DIR}/boost_1_78_0 ${3RDPART_DIR}/boost_1_78_0/stage/lib)
	set(ASIO_DIR ${MODULES_DIR}/libasio ${ROOT_DIR}/build/x64/modules/libasio)
	set(FILELOG_DIR ${MODULES_DIR}/libfilelog)
	
	if(CMAKE_BUILD_TYPE MATCHES "Debug")
		set(DEPENDS 
			${FILELOG_DIR}/${CMAKE_BUILD_TYPE}/filelog.lib 
			${ASIO_DIR}/asio.lib)
	elseif(CMAKE_BUILD_TYPE MATCHES "Release")
		set(DEPENDS 
			${FILELOG_DIR}/${CMAKE_BUILD_TYPE}/filelog.lib 
			${ASIO_DIR}/asio.lib)
	endif()
	
	add_definitions(-DUSE_MODULE_FILE_LOG -DUSE_MODULE_NETWORK_ASIO)
endif()

include_directories(${INCLUDE_DIR} ${BOOST_DIR} ${ASIO_DIR} ${FILELOG_DIR})
file(GLOB_RECURSE src "*.cpp")

add_executable(${PROJECT_NAME} ${src})
target_link_libraries(${PROJECT_NAME} PRIVATE ${DEPENDS})