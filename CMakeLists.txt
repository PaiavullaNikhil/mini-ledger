cmake_minimum_required(VERSION 3.20)

project(MiniLedger
    VERSION 0.2.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(mini-ledger

    app/main.cpp

    src/models/Money.cpp
    src/models/Account.cpp
    src/models/Transaction.cpp
    
    src/storage/Serializer.cpp
    src/storage/StorageEngine.cpp

    src/core/Ledger.cpp
)

target_include_directories(mini-ledger
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)