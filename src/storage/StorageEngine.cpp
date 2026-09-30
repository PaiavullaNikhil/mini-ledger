#include "storage/StorageEngine.h"
#include "storage/Serializer.h"

#include <filesystem>
#include <fstream>
#include <stdexcept>

namespace fs = std::filesystem;

StorageEngine::StorageEngine(
    const std::string& dataDirectory
)
    : dataDirectory(dataDirectory) {

    fs::create_directories(dataDirectory);
}

void StorageEngine::saveAccounts(
    const std::vector<Account>& accounts
) {
    const std::string filePath =
        dataDirectory + "/accounts.dat";

    std::ofstream out(
        filePath,
        std::ios::binary |
        std::ios::trunc
    );

    if (!out) {
        throw std::runtime_error(
            "Unable to open accounts.dat for writing"
        );
    }

    std::int32_t count =
        static_cast<std::int32_t>(accounts.size());

    out.write(
        reinterpret_cast<const char*>(&count),
        sizeof(count)
    );

    for (const auto& account : accounts) {

        Serializer::writeAccount(
            out,
            account
        );
    }
}

std::vector<Account>
StorageEngine::loadAccounts() const {

    const std::string filePath =
        dataDirectory + "/accounts.dat";

    std::ifstream in(
        filePath,
        std::ios::binary
    );

    if (!in) {
        return {};
    }

    std::int32_t count;

    in.read(
        reinterpret_cast<char*>(&count),
        sizeof(count)
    );

    if (!in) {
        throw std::runtime_error(
            "Unable to read account count"
        );
    }

    if (count < 0 || count > 10000000) {
        throw std::runtime_error(
            "Invalid account count"
        );
    }

    std::vector<Account> accounts;
    accounts.reserve(count);

    for (std::int32_t i = 0; i < count; ++i) {

        accounts.push_back(
            Serializer::readAccount(in)
        );
    }

    return accounts;
}

void StorageEngine::saveTransactions(
    const std::vector<Transaction>& transactions
) {
    const std::string filePath =
        dataDirectory + "/transactions.dat";

    std::ofstream out(
        filePath,
        std::ios::binary |
        std::ios::trunc
    );

    if (!out) {
        throw std::runtime_error(
            "Unable to open transactions.dat for writing"
        );
    }

    std::int32_t count =
        static_cast<std::int32_t>(
            transactions.size()
        );

    out.write(
        reinterpret_cast<const char*>(&count),
        sizeof(count)
    );

    for (const auto& transaction :
         transactions) {

        Serializer::writeTransaction(
            out,
            transaction
        );
    }

    if (!out) {
        throw std::runtime_error(
            "Failed to save transactions"
        );
    }
}

std::vector<Transaction>
StorageEngine::loadTransactions() const {

    const std::string filePath =
        dataDirectory + "/transactions.dat";

    std::ifstream in(
        filePath,
        std::ios::binary
    );

    if (!in) {
        return {};
    }

    std::int32_t count;

    in.read(
        reinterpret_cast<char*>(&count),
        sizeof(count)
    );

    if (!in) {
        throw std::runtime_error(
            "Unable to read transaction count"
        );
    }

    if (count < 0 || count > 10000000) {
        throw std::runtime_error(
            "Invalid transaction count"
        );
    }

    std::vector<Transaction> transactions;

    transactions.reserve(count);

    for (std::int32_t i = 0;
         i < count;
         ++i) {

        transactions.push_back(
            Serializer::readTransaction(in)
        );
    }

    return transactions;
}