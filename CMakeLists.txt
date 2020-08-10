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

include(TargetArchitecture)
include(TargetConfiguration)
include(PlatformConfiguration)

include(CheckSSE)
include(CheckAVX)

cm_workspace(boost)

macro(cm_find_package NAME)
    if(NOT "${NAME}" MATCHES "^${CMAKE_WORKSPACE_NAME}_.*$" AND NOT "${NAME}" STREQUAL CM)
        find_package(${ARGV})
    else()
        set(${ARGV0}_FOUND ON CACHE BOOL "")
    endif()
endmacro()

option(BUILD_WITH_CCACHE "Build with ccache usage" TRUE)
option(BUILD_WITH_CRYPTO3_CI_DATA_DRIVEN_TESTS_DISABLED "Build with disabling data-driven tests" FALSE)
option(BUILD_TESTS "Build unit tests" FALSE)

if(BUILD_WITH_CRYPTO3_CI_DATA_DRIVEN_TESTS_DISABLED)
    add_definitions(-DCRYPTO3_CI_DATA_DRIVEN_TESTS_DISABLED)
endif()

set(BUILD_WITH_TARGET_ARCHITECTURE "" CACHE STRING "Target build architecture")

if(BUILD_WITH_TARGET_ARCHITECTURE)
    set(CMAKE_TARGET_ARCHITECTURE ${BUILD_WITH_TARGET_ARCHITECTURE})
else()
    target_architecture(CMAKE_TARGET_ARCHITECTURE)
endif()

define_target_architecture(crypto3 ${CMAKE_TARGET_ARCHITECTURE})
define_target_endianness(crypto3 ${CMAKE_TARGET_ARCHITECTURE})
define_target_platform_features(crypto3)

check_sse()
check_avx()

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