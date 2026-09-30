#pragma once

#include "models/Money.h"

#include <string>
#include <vector>

struct TransactionEntry {
    int accountId;
    Money debit;
    Money credit;
};

class Transaction {
private:
    int id;
    std::string description;
    std::vector<TransactionEntry> entries;

public:
    Transaction(
        int id,
        const std::string& description
    );

    void addEntry(
        int accountId,
        Money debit,
        Money credit
    );

    bool isBalanced() const;

    int getId() const;

    const std::string& getDescription() const;

    const std::vector<TransactionEntry>& getEntries() const;
};