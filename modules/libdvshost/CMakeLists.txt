cmake_minimum_required(VERSION 3.5...3.16)

project(dvshost VERSION "${BUILD_VERSION_STRING}" LANGUAGES CXX)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,-Bsymbolic")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wl,-Bsymbolic")

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
	set(
		HK_ROOT_DIR 
		${3RDPART_DIR}/hikvision/linux/include)
	set(
		HK_LINK_DIR 
		${3RDPART_DIR}/hikvision/linux/lib)
	set(
		LINK_LIBS 
		${HK_LINK_DIR}/libhcnetsdk.so)
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
	set(
		BOOST_ROOT_DIR 
		${3RDPART_DIR}/boost_1_78_0)
	set(
		HK_ROOT_DIR 
		${3RDPART_DIR}/hikvision/win32/include)
	set(
		HK_LINK_DIR 
		${3RDPART_DIR}/hikvision/win32/lib)
	set(
		LINK_LIBS 
		${HK_LINK_DIR}/HCNetSDK.lib)
endif()

set(LIB_ROOT_DIR ${ROOT_DIR}/modules/libdvshost)
include_directories(${INCLUDE_DIR} ${LIB_ROOT_DIR} ${BOOST_ROOT_DIR} ${HK_ROOT_DIR})
file(GLOB_RECURSE main "*.cpp")
file(GLOB_RECURSE 
	utils 
	"${SOURCE_DIR}/utils/memory/*.cpp")

add_library(
	${PROJECT_NAME} 
	SHARED 
	${main}
	${utils})
target_link_libraries(
	${PROJECT_NAME} 
	PRIVATE 
	${LINK_LIBS})