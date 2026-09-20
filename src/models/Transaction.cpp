#include "models/Transaction.h"

Transaction::Transaction(int id, const std::string& description)
    : id(id), description(description) {
}

void Transaction::addEntry(
    int accountId,
    double debit,
    double credit
) {
    entries.push_back({
        accountId,
        debit,
        credit
    });
}

bool Transaction::isBalanced() const {

    double totalDebit = 0.0;
    double totalCredit = 0.0;

    for (const auto& entry : entries) {
        totalDebit += entry.debit;
        totalCredit += entry.credit;
    }

    return totalDebit == totalCredit;
}

int Transaction::getId() const {
    return id;
}

const std::string& Transaction::getDescription() const {
    return description;
}

const std::vector<TransactionEntry>& Transaction::getEntries() const {
    return entries;
}