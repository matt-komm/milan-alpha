project(milan CXX)
cmake_minimum_required(VERSION 2.8.3)
include(cmake/DetectOS.cmake)
include(cmake/Copy.cmake)
include(cmake/Tests.cmake)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

add_definitions(-DOS_${OSNAME})

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
    
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mavx")# -ftree-vectorizer-verbose=9 -fopt-info-vec-optimized")
endif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
    
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")# -Rpass=loop-vectorize -Rpass-analysis=loop-vectorize")
endif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")


if (ENABLE_TEST)
    set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wshadow -pedantic ${CMAKE_CXX_FLAGS}")
endif (ENABLE_TEST)

option(FAST_MATH "Substitute standard math functions with faster versions were applicable" ON)
if (FAST_MATH)
    add_definitions(-DFAST_MATH)
endif(FAST_MATH)

message(STATUS "Compiler id: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "Compiler name: ${CMAKE_CXX_COMPILER}")
message(STATUS "Compiler version: ${CMAKE_CXX_COMPILER_VERSION}")

include_directories(tests)

include_directories(
    SYSTEM external/cpx/include
    SYSTEM external/vdt/include
)

#find_package(ROOT)
#if (ROOT_FOUND)
#    message(STATUS ${ROOT_INCLUDE_DIR})
#    include_directories(SYSTEM ${ROOT_INCLUDE_DIR})
#endif (ROOT_FOUND)

add_subdirectory(external)

include_directories(core/include)
include_directories(algorithms/include)
include_directories(operators/include)
include_directories(plugins/include)

add_subdirectory(core)
add_subdirectory(algorithms)
add_subdirectory(operators)
add_subdirectory(plugins)


