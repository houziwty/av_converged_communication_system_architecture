cmake_minimum_required(VERSION 3.5...3.16)

project(test_libvideo_encode_image VERSION "${BUILD_VERSION_STRING}" LANGUAGES CXX)

include_directories(${INCLUDE_DIR})
include_directories(${SAMPLE_DIR}/libvideo_encode_image)

link_directories(${3RDPART_DIR}/sdk/hikvision/lib/linux)

file(GLOB_RECURSE src "*.cpp")
file(GLOB_RECURSE framework_utils_src "${SOURCE_DIR}/thread/*.cpp" "${SOURCE_DIR}/time/*.cpp")

add_executable(${PROJECT_NAME} ${src} ${framework_utils_src})
target_link_libraries(${PROJECT_NAME} PRIVATE libboost_thread.a video_encode_image)