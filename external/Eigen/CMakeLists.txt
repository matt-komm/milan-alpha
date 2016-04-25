include_directories(SYSTEM include)

add_executable(printEigenConfig
    src/printEigenConfig.cc
)
install(
    TARGETS printEigenConfig
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
)

add_subdirectory(tests)

