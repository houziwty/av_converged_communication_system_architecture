cmake_minimum_required(VERSION 3.5...3.16)

project(dvshost VERSION "${BUILD_VERSION_STRING}" LANGUAGES CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,-Bsymbolic")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wl,-Bsymbolic")

include_directories(${INCLUDE_DIR})
if(CMAKE_SYSTEM_NAME MATCHES "Linux")
	include_directories(${3RDPART_DIR}/sdk/hikvision/include/linux)
	link_directories(${3RDPART_DIR}/sdk/hikvision/lib/linux)
elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")
	include_directories(${3RDPART_DIR}/sdk/hikvision/include/win32)
	link_directories(${3RDPART_DIR}/sdk/hikvision/lib/win32)
endif()

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
	hcnetsdk)