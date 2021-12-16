cmake_minimum_required(VERSION 3.5...3.16)

project(avp VERSION "${BUILD_VERSION_STRING}" LANGUAGES CXX)

include_directories(${INCLUDE_DIR})

file(GLOB_RECURSE src *.cpp)

add_library(${PROJECT_NAME} SHARED ${src})
#target_link_libraries(${PROJECT_NAME} PUBLIC asio：：asio)