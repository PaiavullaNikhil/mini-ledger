add_executable(benchmark_search
    benchmark_search.cpp
    ${PROJECT_SOURCE_DIR}/src/models/Product.cpp
    ${PROJECT_SOURCE_DIR}/src/models/Money.cpp
)

target_include_directories(benchmark_search
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)