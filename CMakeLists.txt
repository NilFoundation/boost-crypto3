cmake_minimum_required(VERSION 2.8.11)

cmake_policy(SET CMP0042 NEW)
cmake_policy(SET CMP0028 NEW)
cmake_policy(SET CMP0048 NEW)
cmake_policy(SET CMP0057 NEW)
cmake_policy(SET CMP0079 NEW)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake"
     "${CMAKE_CURRENT_LIST_DIR}/cmake/packages"
     "${CMAKE_CURRENT_LIST_DIR}/cmake/modules/share/modules/cmake")

include(CMConfig)
include(CMDeploy)
include(CMSetupVersion)

cm_workspace(boost)

macro(cm_find_package NAME)
    if(NOT "${NAME}" MATCHES "^${CMAKE_WORKSPACE_NAME}_.*$" AND NOT "${NAME}" STREQUAL CM)
        find_package(${ARGV})
    else()
        set(${ARGV0}_FOUND ON CACHE BOOL "")
    endif()
endmacro()

option(BUILD_WITH_CCACHE "Build with ccache usage" TRUE)
option(BUILD_TESTS "Build unit tests" FALSE)

configure_file(${CMAKE_CURRENT_LIST_DIR}/cmake/build.hpp.in
               ${CMAKE_BINARY_DIR}/include/nil/${CMAKE_WORKSPACE_NAME}/build.hpp @ONLY)

list(APPEND CMAKE_CURRENT_PROJECTS
     block
     codec
     hash)

foreach(PROJECT_ITERATOR ${CMAKE_CURRENT_PROJECTS})
    include(${PROJECT_ITERATOR})
endforeach()

if(BUILD_TESTS)
    add_subdirectories(test)
endif()