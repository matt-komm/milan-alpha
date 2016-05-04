option(TEST_VDT "Enable testing of external VDT library" OFF)

if (TEST_VDT)
add_gtest(
    NAME external-vdt
    SOURCES testVdt.cc
    LIBRARIES vdt
    #DEPS
)
endif (TEST_VDT)
