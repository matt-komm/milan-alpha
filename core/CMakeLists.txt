include_directories(include)
add_library(milan-core
    src/Histogram.cc
)

add_subdirectory(tests)
