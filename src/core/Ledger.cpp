#include "core/Ledger.h"

#include <iostream>

void Ledger::addAccount(const Account& account) {
    accounts.emplace(account.getId(), account);
}

bool Ledger::hasAccount(int accountId) const {
    return accounts.find(accountId) != accounts.end();
}

Account* Ledger::getAccount(int accountId) {

    auto it = accounts.find(accountId);

    if (it == accounts.end()) {
        return nullptr;
    }

    return &it->second;
}

bool Ledger::postTransaction(
    const Transaction& transaction
) {

    if (!transaction.isBalanced()) {
        return false;
    }

    for (const auto& entry :
         transaction.getEntries()) {

        Account* account =
            getAccount(entry.accountId);

        if (account == nullptr) {
            return false;
        }

        if (!entry.debit.isZero()) {
            account->debit(entry.debit);
        }

        if (!entry.credit.isZero()) {
            account->credit(entry.credit);
        }
    }

    transactions.push_back(transaction);

    return true;
}

void Ledger::printBalances() const {

    for (const auto& [id, account] : accounts) {

        std::cout
            << id
            << " | "
            << account.getName()
            << " | "
            << account.getBalance().getPaise()
            << " paise\n";
    }
}

std::vector<Account> Ledger::getAccounts() const {

    std::vector<Account> result;

    result.reserve(accounts.size());

    for (const auto& [id, account] : accounts) {
        result.push_back(account);
    }

    return result;
}

std::vector<Transaction> Ledger::getTransactions() const {
    return transactions;
}