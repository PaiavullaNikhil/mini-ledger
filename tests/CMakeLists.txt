include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
)

FetchContent_MakeAvailable(googletest)

add_executable(unit_tests
    test_money.cpp
    test_account.cpp
    test_transaction.cpp
    test_ledger.cpp
    test_inventory.cpp
    test_storage.cpp
    test_wal.cpp
    test_index.cpp
)

target_link_libraries(unit_tests
    PRIVATE
        gtest_main
)

target_include_directories(unit_tests
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

target_sources(unit_tests
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src/models/Money.cpp
        ${PROJECT_SOURCE_DIR}/src/models/Account.cpp
        ${PROJECT_SOURCE_DIR}/src/models/Transaction.cpp
        ${PROJECT_SOURCE_DIR}/src/audit/AuditLog.cpp
        ${PROJECT_SOURCE_DIR}/src/models/Product.cpp
        ${PROJECT_SOURCE_DIR}/src/models/Warehouse.cpp
        
        ${PROJECT_SOURCE_DIR}/src/core/Ledger.cpp
        ${PROJECT_SOURCE_DIR}/src/core/Inventory.cpp

        ${PROJECT_SOURCE_DIR}/src/storage/Serializer.cpp
        ${PROJECT_SOURCE_DIR}/src/storage/WAL.cpp
        ${PROJECT_SOURCE_DIR}/src/storage/StorageEngine.cpp
)

include(GoogleTest)

gtest_discover_tests(unit_tests)