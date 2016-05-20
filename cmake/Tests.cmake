OPTION(ENABLE_TEST "Build tests" OFF)
OPTION(ENABLE_MEMTEST "Run memcheck" OFF)
OPTION(ENABLE_VERBOSE_TEST "verbose tests" OFF)

include (CMakeParseArguments)

if (ENABLE_TEST)
    enable_testing()
    if (ENABLE_VERBOSE_TEST)
        add_custom_target(runtests ${CMAKE_CTEST_COMMAND} -V)
    else (ENABLE_VERBOSE_TEST)
        add_custom_target(runtests ${CMAKE_CTEST_COMMAND})
    endif (ENABLE_VERBOSE_TEST)
    add_custom_target(buildtests) 
    add_custom_target(prebuildtests) #use for e.g. copying files before tests are run
    add_dependencies(runtests buildtests)
    add_dependencies(buildtests prebuildtests)
    
    include_directories(BEFORE SYSTEM ${PROJECT_SOURCE_DIR}/external/googletest/include)
    
    macro(add_gtest)
        cmake_parse_arguments(
            TEST                        #prefix
            ""                          #options
            "NAME"                   #one_value_keywords
            "SOURCES;LIBRARIES;DEPS"   #multi_value_keywords
            ${ARGN} 
        )
        add_executable(gtest-${TEST_NAME}
            ${TEST_SOURCES}
        )
        if (NOT ${TEST_LIBRARIES} STREQUAL "")
            target_link_libraries(gtest-${TEST_NAME} ${TEST_LIBRARIES} gtest gtest_main)
        else (NOT ${TEST_LIBRARIES} STREQUAL "")
            target_link_libraries(gtest-${TEST_NAME} gtest gtest_main)
        endif (NOT ${TEST_LIBRARIES} STREQUAL "")
        if (NOT ${TEST_DEPS} STREQUAL "")
            add_dependencies(gtest-${TEST_NAME} ${TEST_DEPS})
        endif (NOT ${TEST_DEPS} STREQUAL "")
        add_test(${TEST_NAME} gtest-${TEST_NAME} --gtest_color=yes)
        if (ENABLE_MEMTEST)
            add_test(
                NAME memtest-${TEST_NAME} 
                COMMAND ${PROJECT_SOURCE_DIR}/cmake/memcheck.py ${CMAKE_CURRENT_BINARY_DIR}/gtest-${TEST_NAME}
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            )
        endif (ENABLE_MEMTEST)
        add_dependencies(runtests gtest-${TEST_NAME})
        install(
            TARGETS gtest-${TEST_NAME}
            RUNTIME DESTINATION bin/tests
            LIBRARY DESTINATION lib
        )
    endmacro(add_gtest)
    
else(ENABLE_TEST)
    macro(add_gtest)
        cmake_parse_arguments(
            TEST                        #prefix
            ""                          #options
            "NAME"                   #one_value_keywords
            "SOURCES;LIBRARIES;DEPS"   #multi_value_keywords
            ${ARGN} 
        )
        message(STATUS "Testing disabled: Skipping test ... ${TEST_NAME}")
    endmacro(add_gtest)
endif(ENABLE_TEST)
