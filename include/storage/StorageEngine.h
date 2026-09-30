#pragma once

#include "models/Account.h"
#include "models/Transaction.h"

#include <string>
#include <vector>

class StorageEngine
{
private:
    std::string dataDirectory;

public:
    explicit StorageEngine(
        const std::string &dataDirectory);

    void saveAccounts(
        const std::vector<Account> &accounts);

    std::vector<Account> loadAccounts() const;

    void saveTransactions(
        const std::vector<Transaction> &transactions);

    std::vector<Transaction> loadTransactions() const;
};