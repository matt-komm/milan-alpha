add_gtest(
    NAME milan-operators-Projection
    SOURCES testProjection.cc
    #LIBRARIES milan-core milan-operators
    #DEPS
)

add_gtest(
    NAME milan-operators-PseudoData
    SOURCES testPseudoData.cc
    #LIBRARIES milan-core milan-operators
    #DEPS
)
