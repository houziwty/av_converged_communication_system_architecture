cmake_minimum_required(VERSION 3.5...3.16)

project(dvshost VERSION "${BUILD_VERSION_STRING}" LANGUAGES CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,-Bsymbolic")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wl,-Bsymbolic")

include_directories(${INCLUDE_DIR})
if(CMAKE_SYSTEM_NAME MATCHES "Linux")
	include_directories(${3RDPART_DIR}/hikvision/linux/include)
	link_directories(${3RDPART_DIR}/hikvision/linux/lib)
elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")
	include_directories(${3RDPART_DIR}/hikvision/win32/include)
	link_directories(${3RDPART_DIR}/hikvision/win32/lib)
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