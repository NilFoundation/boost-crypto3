if(NOT CMAKE_WORKSPACE_NAME OR NOT ("${CMAKE_WORKSPACE_NAME}" STREQUAL "crypto3"))
    cm_workspace(crypto3)
endif()

cm_project(codec WORKSPACE_NAME ${CMAKE_WORKSPACE_NAME} LANGUAGES C CXX)

macro(cm_find_package NAME)
    if(NOT "${NAME}" MATCHES "^${CMAKE_WORKSPACE_NAME}_.*$" AND NOT "${NAME}" STREQUAL CM)
        find_package(${ARGV})
    else()
        set(${ARGV0}_FOUND ON CACHE BOOL "")
    endif()
endmacro()

if(NOT Boost_FOUND AND NOT BUILD_TESTS)
    cm_find_package(Boost REQUIRED COMPONENTS container)
else()
    cm_find_package(Boost REQUIRED COMPONENTS container unit_test_framework)
endif()

option(BUILD_WITH_CCACHE "Build with ccache usage" TRUE)
option(BUILD_TESTS "Build unit tests" FALSE)

list(APPEND ${CURRENT_PROJECT_NAME}_PUBLIC_HEADERS
     include/nil/crypto3/codec/detail/base_policy.hpp
     include/nil/crypto3/codec/detail/hex_policy.hpp

     include/nil/crypto3/detail/exploder.hpp
     include/nil/crypto3/detail/unbounded_shift.hpp
     include/nil/crypto3/detail/imploder.hpp
     include/nil/crypto3/detail/stream_endian.hpp

     include/nil/crypto3/codec/codec_value.hpp
     include/nil/crypto3/codec/codec_state.hpp

     include/nil/crypto3/codec/hex.hpp
     include/nil/crypto3/codec/base.hpp

     include/nil/crypto3/codec/algorithm/encode.hpp
     include/nil/crypto3/codec/algorithm/decode.hpp

     include/nil/crypto3/codec/adaptor/encoded.hpp
     include/nil/crypto3/codec/adaptor/decoded.hpp)

list(APPEND ${CURRENT_PROJECT_NAME}_UNGROUPED_SOURCES)

list(APPEND ${CURRENT_PROJECT_NAME}_HEADERS
     ${${CURRENT_PROJECT_NAME}_PUBLIC_HEADERS})

list(APPEND ${CURRENT_PROJECT_NAME}_SOURCES
     ${${CURRENT_PROJECT_NAME}_UNGROUPED_SOURCES})

cm_setup_version(VERSION 0.1.0 PREFIX ${CMAKE_WORKSPACE_NAME}_${CURRENT_PROJECT_NAME})

add_library(${CMAKE_WORKSPACE_NAME}_${CURRENT_PROJECT_NAME} INTERFACE)

target_link_libraries(${CMAKE_WORKSPACE_NAME}_${CURRENT_PROJECT_NAME} INTERFACE
                      ${Boost_LIBRARIES}
                      "$<TARGET_NAME_IF_EXISTS:boost_multiprecision>")

set_target_properties(${CMAKE_WORKSPACE_NAME}_${CURRENT_PROJECT_NAME} PROPERTIES
                      EXPORT_NAME ${CURRENT_PROJECT_NAME})

target_include_directories(${CMAKE_WORKSPACE_NAME}_${CURRENT_PROJECT_NAME} INTERFACE
                           "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
                           "$<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>"

                           ${Boost_INCLUDE_DIRS})

cm_deploy(TARGETS ${CMAKE_WORKSPACE_NAME}_${CURRENT_PROJECT_NAME}
          INCLUDE include
          NAMESPACE ${CMAKE_WORKSPACE_NAME}::)
