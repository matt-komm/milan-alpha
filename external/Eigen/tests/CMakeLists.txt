#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_RELEASE}")
if (CMAKE_BUILD_TYPE EQUAL "RELEASE")
add_gtest(
    NAME external-Eigen 
    SOURCES testEigen.cc
    #LIBRARIES milan-core
    #DEPS
)
endif (CMAKE_BUILD_TYPE EQUAL "RELEASE")
