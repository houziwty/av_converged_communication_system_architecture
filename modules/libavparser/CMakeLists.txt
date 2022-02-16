cmake_minimum_required(VERSION 3.5...3.16)

project(avparser VERSION "${BUILD_VERSION_STRING}" LANGUAGES CXX)

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
	set(
		BOOST_ROOT_DIR 
		${3RDPART_DIR}/boost_1_78_0)
endif()

set(LIB_ROOT_DIR ${ROOT_DIR}/modules/libavparser)
include_directories(${INCLUDE_DIR} ${LIB_ROOT_DIR} ${BOOST_ROOT_DIR})
file(GLOB_RECURSE main *.cpp)
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