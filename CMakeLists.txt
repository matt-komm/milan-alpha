project(milan CXX)
cmake_minimum_required(VERSION 2.8.3)
include(cmake/DetectOS.cmake)
include(cmake/Copy.cmake)
include(cmake/Tests.cmake)

if (ENABLE_TEST)
    set(CMAKE_CXX_FLAGS "-Wextra -Wall -pedantic -Wno-unused-parameter ${CMAKE_CXX_FLAGS}")
endif (ENABLE_TEST)
    

add_subdirectory(external)


add_subdirectory(core)
