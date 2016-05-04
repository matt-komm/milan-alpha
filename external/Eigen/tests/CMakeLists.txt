option(TEST_EIGEN "Enable testing of external Eigen library" OFF)

if (TEST_EIGEN)
    add_gtest(
        NAME external-Eigen 
        SOURCES testEigen.cc
        #LIBRARIES milan-core
        #DEPS
    )
endif (TEST_EIGEN)
