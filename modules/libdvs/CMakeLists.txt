cmake_minimum_required(VERSION 3.5...3.16)

project(dvs VERSION "${BUILD_VERSION_STRING}" LANGUAGES CXX)

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
	set(HIKVISION_DIR ${3RDPART_DIR}/hikvision/linux/include)
	set(DAHUA_DIR ${3RDPART_DIR}/dahua/linux/include)
	set(DEPENDS ${3RDPART_DIR}/hikvision/linux/lib/libhcnetsdk.so ${3RDPART_DIR}/dahua/linux/lib/libdhnetsdk.so ${3RDPART_DIR}/uniview/linux/lib/libNetDEVSDK.so)
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
	set(BOOST_DIR ${3RDPART_DIR}/boost_1_78_0 ${3RDPART_DIR}/boost_1_78_0/stage/lib)
	set(HIKVISION_DIR ${3RDPART_DIR}/hikvision/x64/include ${3RDPART_DIR}/hikvision/x64/lib)
	set(DAHUA_DIR ${3RDPART_DIR}/dahua/x64/include ${3RDPART_DIR}/dahua/x64/lib)
	set(UNIVIEW_DIR ${3RDPART_DIR}/uniview/x64/include ${3RDPART_DIR}/uniview/x64/lib)
		
	if(CMAKE_BUILD_TYPE MATCHES "Debug")
		set(DEPENDS 
			${BOOST_DIR}/libboost_thread-vc143-mt-sgd-x64-1_78.lib 
			${BOOST_DIR}/libboost_chrono-vc143-mt-sgd-x64-1_78.lib 
			${BOOST_DIR}/libboost_program_options-vc143-mt-sgd-x64-1_78.lib	
			${BOOST_DIR}/libboost_serialization-vc143-mt-sgd-x64-1_78.lib 		
			${HIKVISION_DIR}/HCNetSDK.lib 
			${DAHUA_DIR}/dhnetsdk.lib 
			${UNIVIEW_DIR}/NetDEVSDK.lib)
	elseif(CMAKE_BUILD_TYPE MATCHES "Release")
		set(DEPENDS 
			${BOOST_DIR}/libboost_thread-vc143-mt-s-x64-1_78.lib 
			${BOOST_DIR}/libboost_chrono-vc143-mt-s-x64-1_78.lib 
			${BOOST_DIR}/libboost_program_options-vc143-mt-s-x64-1_78.lib	
			${BOOST_DIR}/libboost_serialization-vc143-mt-s-x64-1_78.lib 		
			${HIKVISION_DIR}/HCNetSDK.lib 
			${DAHUA_DIR}/dhnetsdk.lib 
			${UNIVIEW_DIR}/NetDEVSDK.lib)
	endif()
endif()

set(LIB_DIR ${MODULES_DIR}/libdvs)
include_directories(${INCLUDE_DIR} ${BOOST_DIR} ${HIKVISION_DIR} ${DAHUA_DIR} ${UNIVIEW_DIR} ${LIB_DIR})
file(GLOB_RECURSE src 
	"*.cpp" 
	"${SOURCE_DIR}/memory/*.cpp")

add_library(${PROJECT_NAME} SHARED ${src})
target_link_libraries(${PROJECT_NAME} PRIVATE ${DEPENDS})