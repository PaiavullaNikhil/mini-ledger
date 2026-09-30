#pragma once

#include "models/Account.h"
#include "models/Transaction.h"

#include <unordered_map>

class Ledger {
private:
    std::unordered_map<int, Account> accounts;
    std::vector<Transaction> transactions;

public:
    void addAccount(const Account& account);

    bool hasAccount(int accountId) const;

    Account* getAccount(int accountId);

    bool postTransaction(const Transaction& transaction);

    void printBalances() const;

    std::vector<Account> getAccounts() const;
    std::vector<Transaction> getTransactions() const;
};