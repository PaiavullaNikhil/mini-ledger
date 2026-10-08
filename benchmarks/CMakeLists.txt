add_executable(benchmark_search
    benchmark_search.cpp
    ${PROJECT_SOURCE_DIR}/src/models/Money.cpp
    ${PROJECT_SOURCE_DIR}/src/models/Product.cpp
)

add_executable(benchmark_transactions
    benchmark_transactions.cpp
    ${PROJECT_SOURCE_DIR}/src/models/Money.cpp
    ${PROJECT_SOURCE_DIR}/src/models/Account.cpp
    ${PROJECT_SOURCE_DIR}/src/models/Transaction.cpp
    ${PROJECT_SOURCE_DIR}/src/core/Ledger.cpp
    ${PROJECT_SOURCE_DIR}/src/audit/AuditLog.cpp
    ${PROJECT_SOURCE_DIR}/src/storage/Serializer.cpp
    ${PROJECT_SOURCE_DIR}/src/storage/WAL.cpp
)

add_executable(benchmark_storage
    benchmark_storage.cpp
    ${PROJECT_SOURCE_DIR}/src/models/Money.cpp
    ${PROJECT_SOURCE_DIR}/src/models/Account.cpp
    ${PROJECT_SOURCE_DIR}/src/models/Transaction.cpp
    ${PROJECT_SOURCE_DIR}/src/storage/Serializer.cpp
    ${PROJECT_SOURCE_DIR}/src/storage/StorageEngine.cpp
)

foreach(benchmark_target IN ITEMS
    benchmark_search
    benchmark_transactions
    benchmark_storage
)
    target_include_directories(${benchmark_target}
        PRIVATE
            ${PROJECT_SOURCE_DIR}/include
    )
endforeach()