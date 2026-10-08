add_executable(benchmark_transactions
    benchmark_storage.cpp
    ${PROJECT_SOURCE_DIR}/src/models/Money.cpp
    ${PROJECT_SOURCE_DIR}/src/models/Account.cpp
    ${PROJECT_SOURCE_DIR}/src/models/Transaction.cpp
    ${PROJECT_SOURCE_DIR}/src/core/Ledger.cpp
    ${PROJECT_SOURCE_DIR}/src/storage/Serializer.cpp
    ${PROJECT_SOURCE_DIR}/src/storage/WAL.cpp
    ${PROJECT_SOURCE_DIR}/src/audit/AuditLog.cpp
    ${PROJECT_SOURCE_DIR}/src/storage/StorageEngine.cpp
)

target_include_directories(benchmark_transactions
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)