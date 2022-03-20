cmake_minimum_required(VERSION 3.5...3.16)

project(http_host_server VERSION "${BUILD_VERSION_STRING}" LANGUAGES CXX)

set(
	ASIO_ROOT_DIR 
	${ROOT_DIR}/modules/libasio)
set(
	HTTP_ROOT_DIR 
	${ROOT_DIR}/modules/libhttp)

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
	link_directories(${XMQ_ROOT_DIR} ${ASIO_ROOT_DIR} ${HTTP_ROOT_DIR})
	set(
		LINK_LIBS 
		libboost_program_options.a 
		pthread 
		asio 
		http)
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
	set(
		BOOST_ROOT_DIR 
		${3RDPART_DIR}/boost_1_78_0)
	set(
		BOOST_LINK_DIR 
		${BOOST_ROOT_DIR}/stage/lib)
	set(
		ASIO_LINK_DIR 
		${ROOT_DIR}/build/x64/modules/libasio/${CMAKE_BUILD_TYPE})
	set(
		HTTP_LINK_DIR 
		${ROOT_DIR}/build/x64/modules/libhttp/${CMAKE_BUILD_TYPE})
	if(CMAKE_BUILD_TYPE MATCHES "Debug")
		set(
			LINK_LIBS 
			${BOOST_LINK_DIR}/libboost_thread-vc143-mt-sgd-x64-1_78.lib 
			${BOOST_LINK_DIR}/libboost_chrono-vc143-mt-sgd-x64-1_78.lib 
			${BOOST_LINK_DIR}/libboost_program_options-vc143-mt-sgd-x64-1_78.lib	
			${BOOST_LINK_DIR}/libboost_serialization-vc143-mt-sgd-x64-1_78.lib 
			${ASIO_LINK_DIR}/asio.lib 
			${HTTP_LINK_DIR}/http.lib)
	elseif(CMAKE_BUILD_TYPE MATCHES "Release")
		set(
			LINK_LIBS 
			${BOOST_LINK_DIR}/libboost_thread-vc143-mt-s-x64-1_78.lib 
			${BOOST_LINK_DIR}/libboost_chrono-vc143-mt-s-x64-1_78.lib 
			${BOOST_LINK_DIR}/libboost_program_options-vc143-mt-s-x64-1_78.lib	
			${BOOST_LINK_DIR}/libboost_serialization-vc143-mt-s-x64-1_78.lib 
			${ASIO_LINK_DIR}/asio.lib 
			${DVS_LINK_DIR}/http.lib)
	endif()
	add_definitions(-DUSE_MODULE_NETWORK_ASIO -DUSE_MODULE_HTTP)
endif()

include_directories(
	${INCLUDE_DIR} 
	${BOOST_ROOT_DIR} 
	${ASIO_ROOT_DIR}  
	${HTTP_ROOT_DIR})
file(GLOB_RECURSE main "*.cpp")
file(GLOB_RECURSE 
	utils 
	"${SOURCE_DIR}/commandline/*.cpp")

add_executable(
	${PROJECT_NAME} 
	${main} 
	${utils})
target_link_libraries(
	${PROJECT_NAME} 
	PRIVATE 
	${LINK_LIBS})