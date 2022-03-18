cmake_minimum_required(VERSION 3.5...3.16)

project(database_host_server VERSION "${BUILD_VERSION_STRING}" LANGUAGES CXX)

set(
	DB_ROOT_DIR 
	${ROOT_DIR}/modules/libdatabase)
set(
	LOG_ROOT_DIR 
	${ROOT_DIR}/modules/liblog)

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
	link_directories(${DB_ROOT_DIR} ${LOG_ROOT_DIR})
	set(
		LINK_LIBS 
		database 
		log)
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
	set(
		BOOST_ROOT_DIR 
		${3RDPART_DIR}/boost_1_78_0)
	set(
		BOOST_LINK_DIR 
		${BOOST_ROOT_DIR}/stage/lib)
	set(
		DB_LINK_DIR 
		${ROOT_DIR}/build/x64/modules/libdatabase/${CMAKE_BUILD_TYPE})
	set(
		LOG_LINK_DIR 
		${ROOT_DIR}/build/x64/modules/liblog/${CMAKE_BUILD_TYPE})
	if(CMAKE_BUILD_TYPE MATCHES "Debug")
		set(
			LINK_LIBS 
			${BOOST_LINK_DIR}/libboost_program_options-vc143-mt-sgd-x64-1_78.lib 
			${BOOST_LINK_DIR}/libboost_filesystem-vc143-mt-sgd-x64-1_78.lib 
			${DB_LINK_DIR}/database.lib 
			${LOG_LINK_DIR}/log.lib)
	elseif(CMAKE_BUILD_TYPE MATCHES "Release")
		set(
			LINK_LIBS 
			${BOOST_LINK_DIR}/libboost_program_options-vc143-mt-s-x64-1_78.lib 
			${BOOST_LINK_DIR}/libboost_filesystem-vc143-mt-s-x64-1_78.lib 
			${DB_LINK_DIR}/database.lib 
			${LOG_LINK_DIR}/log.lib)
	endif()
	add_definitions(-DUSE_MODULE_FILE_LOG -DUSE_MODULE_FILE_DATABASE)
endif()

include_directories(
	${INCLUDE_DIR} 
	${BOOST_ROOT_DIR} 
	${DB_ROOT_DIR} 
	${LOG_ROOT_DIR})
file(GLOB_RECURSE main "*.cpp")

add_executable(
	${PROJECT_NAME} 
	${main})
target_link_libraries(
	${PROJECT_NAME} 
	PRIVATE 
	${LINK_LIBS})