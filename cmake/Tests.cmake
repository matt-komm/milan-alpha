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

        target_link_libraries(gtest-${TEST_NAME} ${TEST_LIBRARIES} gtest gtest_main)
        
        if (TEST_DEPS)
            add_dependencies(gtest-${TEST_NAME} ${TEST_DEPS})
        endif (TEST_DEPS)
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
    
    if ( CMAKE_MAJOR_VERSION EQUAL 3)
        macro(add_compile_ftest)
            cmake_parse_arguments(
                TEST                        #prefix
                ""                          #options
                "NAME"                   #one_value_keywords
                "SOURCES;LIBRARIES;DEPS;DEFS"   #multi_value_keywords
                ${ARGN} 
            )
            
            foreach(DEF ${TEST_DEFS})
            
                add_executable(ftest-${TEST_NAME}-${DEF}
                    ${TEST_SOURCES}
                )
                set_target_properties(ftest-${TEST_NAME}-${DEF}
                    PROPERTIES
                    EXCLUDE_FROM_ALL TRUE
                    EXCLUDE_FROM_DEFAULT_BUILD TRUE
                )
                target_compile_definitions(ftest-${TEST_NAME}-${DEF} PRIVATE ${DEF})

                target_link_libraries(ftest-${TEST_NAME}-${DEF} ${TEST_LIBRARIES})
                
                if (TEST_DEPS)
                    add_dependencies(ftest-${TEST_NAME}-${DEF} ${TEST_DEPS})
                endif (TEST_DEPS)
                add_test(NAME ${TEST_NAME}-${DEF}
                    COMMAND ${CMAKE_COMMAND} --build . --target ftest-${TEST_NAME}-${DEF} --config $<CONFIGURATION>
                    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                )
                set_tests_properties(${TEST_NAME}-${DEF} PROPERTIES WILL_FAIL TRUE)
                
            endforeach(DEF)
            
        endmacro(add_compile_ftest)
    else ( CMAKE_MAJOR_VERSION EQUAL 3)
        macro(add_compile_ftest)
            cmake_parse_arguments(
                TEST                        #prefix
                ""                          #options
                "NAME"                   #one_value_keywords
                "SOURCES;LIBRARIES;DEPS"   #multi_value_keywords
                ${ARGN} 
            )
            message(STATUS "Compile failure testing disabled (requires cmake 3): Skipping test ... ${TEST_NAME}")
        endmacro(add_compile_ftest)
    endif ( CMAKE_MAJOR_VERSION EQUAL 3)
    
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
    
    macro(add_compile_ftest)
        cmake_parse_arguments(
            TEST                        #prefix
            ""                          #options
            "NAME"                   #one_value_keywords
            "SOURCES;LIBRARIES;DEPS"   #multi_value_keywords
            ${ARGN} 
        )
        message(STATUS "Testing disabled: Skipping test ... ${TEST_NAME}")
    endmacro(add_compile_ftest)
endif(ENABLE_TEST)


