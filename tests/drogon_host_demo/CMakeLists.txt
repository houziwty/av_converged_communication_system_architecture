cmake_minimum_required(VERSION 3.5...3.16)

project(drogon_host_demo VERSION "${BUILD_VERSION_STRING}" LANGUAGES CXX)

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
	set(DEPENDS libdrogon.a libjsoncpp.a libtrantor.a libredis++.a libhiredis_static.a pthread z uuid dl crypto ssl)
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
endif()

file(GLOB_RECURSE src "*.cpp")

add_executable(${PROJECT_NAME} ${src})
target_link_libraries(${PROJECT_NAME} PRIVATE ${DEPENDS})