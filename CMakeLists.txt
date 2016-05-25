project(milan CXX)
cmake_minimum_required(VERSION 2.8.3)
include(cmake/DetectOS.cmake)
include(cmake/Copy.cmake)
include(cmake/Tests.cmake)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
    
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mavx")
endif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
    
    #set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mavx")
endif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")


if (ENABLE_TEST)
    set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic ${CMAKE_CXX_FLAGS}")
endif (ENABLE_TEST)

message(STATUS "Compiler id: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "Compiler name: ${CMAKE_CXX_COMPILER}")
message(STATUS "Compiler version: ${CMAKE_CXX_COMPILER_VERSION}")

add_subdirectory(external)

include_directories(
    SYSTEM ${PROJECT_SOURCE_DIR}/external/vectorclass
    SYSTEM ${PROJECT_SOURCE_DIR}/external/Eigen/include
)

include_directories(core/include)
include_directories(operators/include)

add_subdirectory(core)
add_subdirectory(operators)

