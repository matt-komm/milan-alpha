include_directories(SYSTEM include)

add_executable(printSimdSupport
    src/printSimdSupport.cc
)
install(
    TARGETS printSimdSupport
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
)

add_subdirectory(tests)

