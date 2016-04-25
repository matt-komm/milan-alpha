include_directories(SYSTEM include)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})

add_executable(printSimdSupport
    src/printSimdSupport.cc
)

add_subdirectory(tests)

