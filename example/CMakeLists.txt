#---------------------------------------------------------------------------#
# Copyright (c) 2020 Mikhail Komarov <nemo@nil.foundation>
#
# Distributed under the Boost Software License, Version 1.0
# See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt
#---------------------------------------------------------------------------#

include(CMTest)

cm_find_package(${CMAKE_WORKSPACE_NAME}_block)

if(NOT Boost_UNIT_TEST_FRAMEWORK_FOUND OR NOT Boost_FILESYSTEM_FOUND)
    cm_find_package(Boost REQUIRED COMPONENTS unit_test_framework filesystem)
endif()

cm_test_link_libraries(${CMAKE_WORKSPACE_NAME}_${CURRENT_PROJECT_NAME}
                       ${CMAKE_WORKSPACE_NAME}::block
                       ${Boost_LIBRARIES})

macro(define_example name)
    cm_test(NAME ${name}_example SOURCES ${name}.cpp)

    target_include_directories(${name}_example PRIVATE
                               "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
                               "$<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>"

                               ${Boost_INCLUDE_DIRS})

    set_target_properties(${name}_example PROPERTIES
                          CXX_STANDARD 14
                          CXX_STANDARD_REQUIRED TRUE)

    get_target_property(target_type Boost::unit_test_framework TYPE)
    if(target_type STREQUAL "SHARED_LIB")
        target_compile_definitions(${name}_example PRIVATE BOOST_TEST_DYN_LINK)
    elseif(target_type STREQUAL "STATIC_LIB")

    endif()
endmacro()

set(EXAMPLES_NAMES
    "encrypt")

foreach(EXAMPLE_NAME ${EXAMPLES_NAMES})
    define_example(${EXAMPLE_NAME})
endforeach()
