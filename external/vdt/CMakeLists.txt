include_directories(SYSTEM include)
add_library(vdt
    src/vdtMath_signatures
)

add_subdirectory(tests)
