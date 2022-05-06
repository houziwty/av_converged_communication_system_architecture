cmake_minimum_required(VERSION 3.5...3.16)

project(avccsa)

#项目版本信息
include(cmake/utils.cmake)
extract_build_version()

#set(CMAKE_BUILD_TYPE "Release")
if ("${CMAKE_BUILD_TYPE}" STREQUAL "")
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
    set(CMAKE_BUILD_TYPE "Debug")
endif ()

message(STATUS "******   Project Version : ${PROJECT_NAME}_${BUILD_VERSION_STRING}")
message(STATUS "******   System Name : ${CMAKE_SYSTEM_NAME}")
message(STATUS "******   Build Type : ${CMAKE_BUILD_TYPE}")

if(MSVC)
    add_compile_options(
        $<$<CONFIG:>:/MT> #---------|
        $<$<CONFIG:Debug>:/MTd> #---|-- Statically link the runtime libraries
        $<$<CONFIG:Release>:/MT> #--|
		)
endif()

set(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(3RDPART_DIR ${ROOT_DIR}/3rd)
set(INCLUDE_DIR ${ROOT_DIR}/include)
set(SOURCE_DIR ${ROOT_DIR}/src)
set(MODULES_DIR ${ROOT_DIR}/modules)

add_subdirectory(modules/libfilelog)
add_subdirectory(modules/libasio)
add_subdirectory(modules/libxmq)
add_subdirectory(modules/libavcodec)
add_subdirectory(modules/libavparser)
add_subdirectory(modules/libavpkt)
add_subdirectory(modules/libavplayer)
add_subdirectory(modules/libav)
add_subdirectory(modules/libdvs)
if(CMAKE_SYSTEM_NAME MATCHES "Linux")
add_subdirectory(modules/libfdfs)
add_subdirectory(services/storage_host_server)
endif()
#add_subdirectory(modules/libhttp)
#add_subdirectory(modules/libdatabase)
add_subdirectory(services/xmq_host_server)
add_subdirectory(services/dvs_host_server)
add_subdirectory(services/log_host_server)
#add_subdirectory(services/http_host_server)
#add_subdirectory(services/database_host_server)
add_subdirectory(benchmarks/xmq_pub_benchmarks)
add_subdirectory(benchmarks/xmq_sub_benchmarks)
add_subdirectory(benchmarks/xmq_send_benchmarks)
#add_subdirectory(benchmarks/storage_upload_benchmarks)
