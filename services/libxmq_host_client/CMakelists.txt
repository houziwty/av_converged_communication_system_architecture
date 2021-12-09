cmake_minimum_required(VERSION 3.5...3.16)

project(dvs_host_client VERSION "${BUILD_VERSION_STRING}" LANGUAGES CXX)

include_directories(${INCLUDE_DIR})
link_directories(${LINK_DIR}/libxmq)
link_directories(${LINK_DIR}/libasio)

file(GLOB_RECURSE main "*.cpp")
file(GLOB_RECURSE xmq "${SOURCE_DIR}/network/xmq/worker_model.cpp")
file(GLOB_RECURSE utils "${SOURCE_DIR}/utils/uuid/*.cpp")

add_library(${PROJECT_NAME} SHARED ${main} ${xmq} ${utils})
target_link_libraries(${PROJECT_NAME} PRIVATE libboost_thread.a xmq asio)