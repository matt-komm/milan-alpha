set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -mavx")
#set(CMAKE_CXX_FLAGS "-S ${CMAKE_CXX_FLAGS}")

add_gtest(
    NAME milan-core-Histogram 
    SOURCES testHistogram.cc
    LIBRARIES milan-core
    #DEPS
)


