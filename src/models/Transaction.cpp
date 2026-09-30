#include "models/Transaction.h"
#include <stdexcept>

Transaction::Transaction(
    int id,
    const std::string& description
)
    : id(id),
      description(description) {
}

void Transaction::addEntry(
    int accountId,
    Money debit,
    Money credit
) {

    if (!debit.isZero() && !credit.isZero()) {
        throw std::invalid_argument(
            "An entry cannot contain both debit and credit"
        );
    }

    if (debit.isZero() && credit.isZero()) {
        throw std::invalid_argument(
            "An entry must contain either debit or credit"
        );
    }

    entries.push_back({
        accountId,
        debit,
        credit
    });
}

bool Transaction::isBalanced() const {

    Money totalDebit(0);
    Money totalCredit(0);

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

const std::vector<TransactionEntry>&
Transaction::getEntries() const {
    return entries;
}