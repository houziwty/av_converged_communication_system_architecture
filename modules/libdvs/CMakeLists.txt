cmake_minimum_required(VERSION 3.5...3.16)

project(dvs VERSION "${BUILD_VERSION_STRING}" LANGUAGES CXX)

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
	set(
		HK_ROOT_DIR 
		${3RDPART_DIR}/hikvision/linux/include)
	set(
		HK_LINK_DIR 
		${3RDPART_DIR}/hikvision/linux/lib)
	set(
		DH_ROOT_DIR 
		${3RDPART_DIR}/dahua/linux/include)
	set(
		DH_LINK_DIR 
		${3RDPART_DIR}/dahua/linux/lib)	
	set(
		LINK_LIBS 
		${HK_LINK_DIR}/libhcnetsdk.so 
		${DH_LINK_DIR}/libdhnetsdk.so)
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
	set(
		BOOST_ROOT_DIR 
		${3RDPART_DIR}/boost_1_78_0)
	set(
		BOOST_LINK_DIR 
		${BOOST_ROOT_DIR}/stage/lib)
	set(
		HK_ROOT_DIR 
		${3RDPART_DIR}/hikvision/x64/include)
	set(
		HK_LINK_DIR 
		${3RDPART_DIR}/hikvision/x64/lib)
	set(
		DH_ROOT_DIR 
		${3RDPART_DIR}/dahua/x64/include)
	set(
		DH_LINK_DIR 
		${3RDPART_DIR}/dahua/x64/lib)
		
	if(CMAKE_BUILD_TYPE MATCHES "Debug")
		set(
			LINK_LIBS 
			${BOOST_LINK_DIR}/libboost_thread-vc143-mt-sgd-x64-1_78.lib 
			${BOOST_LINK_DIR}/libboost_chrono-vc143-mt-sgd-x64-1_78.lib 
			${BOOST_LINK_DIR}/libboost_program_options-vc143-mt-sgd-x64-1_78.lib	
			${BOOST_LINK_DIR}/libboost_serialization-vc143-mt-sgd-x64-1_78.lib 		
			${HK_LINK_DIR}/HCNetSDK.lib 
			${DH_LINK_DIR}/dhnetsdk.lib)
	elseif(CMAKE_BUILD_TYPE MATCHES "Release")
		set(
			LINK_LIBS 
			${BOOST_LINK_DIR}/libboost_thread-vc143-mt-s-x64-1_78.lib 
			${BOOST_LINK_DIR}/libboost_chrono-vc143-mt-s-x64-1_78.lib 
			${BOOST_LINK_DIR}/libboost_program_options-vc143-mt-s-x64-1_78.lib	
			${BOOST_LINK_DIR}/libboost_serialization-vc143-mt-s-x64-1_78.lib 		
			${HK_LINK_DIR}/HCNetSDK.lib 
			${DH_LINK_DIR}/dhnetsdk.lib)
	endif()
endif()

set(LIB_ROOT_DIR ${ROOT_DIR}/modules/libdvs)
include_directories(
	${INCLUDE_DIR} 
	${LIB_ROOT_DIR} 
	${BOOST_ROOT_DIR} 
	${HK_ROOT_DIR} 
	${DH_ROOT_DIR})
file(GLOB_RECURSE main "*.cpp")
file(GLOB_RECURSE 
	utils 
	"${SOURCE_DIR}/memory/*.cpp")

add_library(
	${PROJECT_NAME} 
	SHARED 
	${main}
	${utils})
target_link_libraries(
	${PROJECT_NAME} 
	PRIVATE 
	${LINK_LIBS})