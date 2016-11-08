add_gtest(
    NAME milan-core-Binning
    SOURCES testBinning.cc
    LIBRARIES milan-core
    #DEPS
)

add_gtest(
    NAME milan-core-Histogram 
    SOURCES testHistogram.cc
    LIBRARIES milan-core
    #DEPS
)

add_gtest(
    NAME milan-core-ParametricHistogram 
    SOURCES testParametricHistogram.cc
    LIBRARIES milan-core
    #DEPS
)

add_gtest(
    NAME milan-core-HistogramFunction
    SOURCES testHistogramFunction.cc
    LIBRARIES milan-core
    #DEPS
)

add_gtest(
    NAME milan-core-Ptr
    SOURCES testPtr.cc
    #LIBRARIES milan-core
    #DEPS
)

add_compile_ftest(
    NAME milan-core-Ptr
    SOURCES testPtr-fail.cc
    #LIBRARIES milan-core
    DEFS COPY COPY_ASSIGN MOVE MOVE_ASSIGN
)

add_gtest(
    NAME milan-core-Parameter
    SOURCES testParameter.cc
    #LIBRARIES milan-core
    #DEPS
)

add_gtest(
    NAME milan-core-ScalarConstant
    SOURCES testScalarConstant.cc
    #LIBRARIES milan-core
    #DEPS
)

add_gtest(
    NAME milan-core-Function
    SOURCES testFunction.cc
    #LIBRARIES milan-core
    #DEPS
)

add_gtest(
    NAME milan-core-Likelihood
    SOURCES testLikelihood.cc
    LIBRARIES vdt
    #DEPS
)


