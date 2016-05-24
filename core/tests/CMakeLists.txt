add_gtest(
    NAME milan-core-Histogram 
    SOURCES testHistogram.cc
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

add_gtest(
    NAME milan-core-Parameter
    SOURCES testParameter.cc
    #LIBRARIES milan-core
    #DEPS
)

add_gtest(
    NAME milan-core-Likelihood
    SOURCES testLikelihood.cc
    #LIBRARIES milan-core
    #DEPS
)

add_gtest(
    NAME milan-core-Projection
    SOURCES testProjection.cc
    #LIBRARIES milan-core
    #DEPS
)
