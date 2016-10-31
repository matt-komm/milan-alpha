option(TEST_FADBAD "Enable testing of external fadbad library" OFF)

if (TEST_FADBAD)
    add_gtest(
        NAME external-fadbad
        SOURCES testFadbad.cc
        #LIBRARIES milan-core
        #DEPS
    )
endif (TEST_FADBAD)
