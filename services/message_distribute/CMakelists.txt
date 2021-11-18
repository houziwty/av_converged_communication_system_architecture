cmake_minimum_required(VERSION 3.5...3.16)

project(message_distribute VERSION "${BUILD_VERSION_STRING}" LANGUAGES CXX)

include_directories(${INCLUDE_DIR})
link_directories(${LINK_DIR}/libxmq)

aux_source_directory(. main_src)
aux_source_directory(${SOURCE_DIR}/network/mq framework_network_mq_src)
aux_source_directory(${SOURCE_DIR}/utils/thread framework_utils_thread_src)
aux_source_directory(${SOURCE_DIR}/utils/hardware framework_utils_hardware_src)

add_executable(
	${PROJECT_NAME} 
	${main_src} 
	${framework_network_mq_src} 
	${framework_utils_thread_src}
	${framework_utils_hardware_src})
target_link_libraries(${PROJECT_NAME} PRIVATE xmq libboost_thread.a pthread)