add_gtest(
    NAME milan-algorithms-RandomEngine
    SOURCES testRandomEngine.cc
    #LIBRARIES milan-core
    #DEPS
)

add_gtest(
    NAME milan-algorithms-MLFitter
    SOURCES testMLFitter.cc
    #LIBRARIES milan-core
    #DEPS
)

if(ROOT_FOUND)
    add_gtest(
        NAME milan-algorithms-HMC
        SOURCES testHMC.cc
        LIBRARIES ${ROOT_LIBRARIES}
        #DEPS
    )
endif(ROOT_FOUND)
