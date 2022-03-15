cmake_minimum_required(VERSION 3.5...3.16)

project(video_encode_image VERSION "${BUILD_VERSION_STRING}" LANGUAGES CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,-Bsymbolic")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wl,-Bsymbolic")

include_directories(${INCLUDE_DIR})
include_directories(${3RDPART_DIR}/sdk/hikvision/include/linux)

link_directories(${3RDPART_DIR}/sdk/hikvision/lib/linux)

file(GLOB_RECURSE src "*.cpp")
file(GLOB_RECURSE framework_utils_src "${SOURCE_DIR}/memory/*.cpp")

add_library(
	${PROJECT_NAME} 
	SHARED ${src} 
	${framework_utils_src})
target_link_libraries(
	${PROJECT_NAME} 
	PUBLIC 
	libavcodec.a 
	libavformat.a 
	libavutil.a 
	libswscale.a 
	hcnetsdk 
	PlayCtrl 
	pthread)