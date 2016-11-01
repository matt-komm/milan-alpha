option(TEST_ADEPT "Enable testing of external adept library" OFF)

if (TEST_ADEPT)
    add_gtest(
        NAME external-adept
        SOURCES testAdept.cc
        LIBRARIES adept
        #DEPS
    )
endif (TEST_ADEPT)
