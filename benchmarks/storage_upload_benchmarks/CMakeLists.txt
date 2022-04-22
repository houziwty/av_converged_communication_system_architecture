cmake_minimum_required(VERSION 3.5...3.16)

project(storage_upload_benchmarks VERSION "${BUILD_VERSION_STRING}" LANGUAGES CXX)
	
if(CMAKE_SYSTEM_NAME MATCHES "Linux")
	set(FDFS_DIR ${MODULES_DIR}/libfdfs)
	set(DEPENDS libboost_thread.a pthread fdfs)
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
	set(BOOST_DIR ${3RDPART_DIR}/boost_1_78_0 ${3RDPART_DIR}/boost_1_78_0/stage/lib)
	set(FDFS_DIR ${MODULES_DIR}/libfdfs ${ROOT_DIR}/build/x64/modules/libfdfs)
	
	if(CMAKE_BUILD_TYPE MATCHES "Debug")
		set(DEPENDS 
			${BOOST_DIR}/libboost_thread-vc143-mt-sgd-x64-1_78.lib 
			${BOOST_DIR}/libboost_chrono-vc143-mt-sgd-x64-1_78.lib 
			${BOOST_DIR}/libboost_program_options-vc143-mt-sgd-x64-1_78.lib	
			${BOOST_DIR}/libboost_serialization-vc143-mt-sgd-x64-1_78.lib 		
			${FDFS_DIR}/${CMAKE_BUILD_TYPE}/fdfs.lib)
	elseif(CMAKE_BUILD_TYPE MATCHES "Release")
		set(DEPENDS 
			${BOOST_DIR}/libboost_thread-vc143-mt-s-x64-1_78.lib 
			${BOOST_DIR}/libboost_chrono-vc143-mt-s-x64-1_78.lib 
			${BOOST_DIR}/libboost_program_options-vc143-mt-s-x64-1_78.lib	
			${BOOST_DIR}/libboost_serialization-vc143-mt-s-x64-1_78.lib 		
			${FDFS_DIR}/${CMAKE_BUILD_TYPE}/fdfs.lib)
	endif()
	
	add_definitions(-DUSE_MODULE_FILE_FDFS -DUSE_MODULE_AV_PKT)
endif()

include_directories(${INCLUDE_DIR} ${BOOST_DIR} ${FDFS_DIR})
file(GLOB_RECURSE src 
	"*.cpp"
	"${SOURCE_DIR}/time/*.cpp" 
	"${SOURCE_DIR}/url/*.cpp" 
	"${SOURCE_DIR}/memory/*.cpp" 
	"${SOURCE_DIR}/thread/*.cpp")

add_executable(${PROJECT_NAME} ${src})
target_link_libraries(${PROJECT_NAME} PRIVATE ${DEPENDS})