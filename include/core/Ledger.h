#pragma once

#include "audit/AuditLog.h"
#include "models/Account.h"
#include "models/Transaction.h"

#include <unordered_map>
#include <vector>

class Ledger {
private:
    std::unordered_map<int, Account> accounts;
    std::vector<Transaction> transactions;
    AuditLog* auditLog;

public:
    explicit Ledger(
        AuditLog* auditLog = nullptr
    );

    void addAccount(const Account& account);

    bool hasAccount(int accountId) const;

    Account* getAccount(int accountId);

    const Account* getAccount(int accountId) const;

    bool postTransaction(
        const Transaction& transaction
    );

    std::vector<Account> getAccounts() const;

    std::vector<Transaction> getTransactions() const;

    void printBalances() const;
};