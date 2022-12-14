function(extract_build_version)
    file(READ "${CMAKE_CURRENT_SOURCE_DIR}/version.h.in" content)
    string(REGEX MATCH "MAJOR ([0-9]+)" _  "${content}")
    if(NOT CMAKE_MATCH_COUNT EQUAL 1)
        message(FATAL_ERROR "Could not extract major version number from version.h")
    endif()
    set(major ${CMAKE_MATCH_1})

    string(REGEX MATCH "MINOR ([0-9]+)" _  "${content}")
    if(NOT CMAKE_MATCH_COUNT EQUAL 1)
        message(FATAL_ERROR "Could not extract minor version number from version.h")
    endif()
    set(minor ${CMAKE_MATCH_1})

    string(REGEX MATCH "PATCH ([0-9]+)" _  "${content}")
    if(NOT CMAKE_MATCH_COUNT EQUAL 1)
        message(FATAL_ERROR "Could not extract patch version number from version.h")
    endif()
    set(patch ${CMAKE_MATCH_1})

    #set(COMMON_VERSION_MAJOR ${verion_major} PARENT_SCOPE)
    set(BUILD_VERSION_STRING "${major}.${minor}.${patch}" PARENT_SCOPE)
endfunction()