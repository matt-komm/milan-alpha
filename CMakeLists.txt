project(milan CXX C)
cmake_minimum_required(VERSION 2.8.3)
include(cmake/DetectOS.cmake)
include(cmake/Copy.cmake)
include(cmake/Tests.cmake)

add_subdirectory(external)

add_subdirectory(core)
