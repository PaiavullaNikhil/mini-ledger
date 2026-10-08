#include "storage/StorageEngine.h"
#include "models/Account.h"
#include "models/Transaction.h"
#include "models/Money.h"

#include <chrono>
#include <iostream>
#include <vector>

int main()
{
    const int accountCount = 10'000;
    const int transactionCount = 10'000;

    std::vector<Account> accounts;
    accounts.reserve(accountCount);

    for (int i = 0; i < accountCount; ++i)
    {
        accounts.emplace_back(
            i,
            "Account",
            AccountType::ASSET
        );
    }

    std::vector<Transaction> transactions;
    transactions.reserve(transactionCount);

    for (int i = 0; i < transactionCount; ++i)
    {
        Transaction transaction(
            i,
            "Benchmark transaction"
        );

        transaction.addEntry(
            0,
            Money(100),
            Money(0)
        );

        transaction.addEntry(
            1,
            Money(0),
            Money(100)
        );

        transactions.push_back(transaction);
    }

    StorageEngine storage("data/benchmark");

    auto startAccountSave =
        std::chrono::high_resolution_clock::now();

    storage.saveAccounts(accounts);

    auto endAccountSave =
        std::chrono::high_resolution_clock::now();

    auto startAccountLoad =
        std::chrono::high_resolution_clock::now();

    auto loadedAccounts =
        storage.loadAccounts();

    auto endAccountLoad =
        std::chrono::high_resolution_clock::now();

    auto startTransactionSave =
        std::chrono::high_resolution_clock::now();

    storage.saveTransactions(transactions);

    auto endTransactionSave =
        std::chrono::high_resolution_clock::now();

    auto startTransactionLoad =
        std::chrono::high_resolution_clock::now();

    auto loadedTransactions =
        storage.loadTransactions();

    auto endTransactionLoad =
        std::chrono::high_resolution_clock::now();

    auto accountSaveTime =
        std::chrono::duration_cast<
            std::chrono::milliseconds
        >(endAccountSave - startAccountSave);

    auto accountLoadTime =
        std::chrono::duration_cast<
            std::chrono::milliseconds
        >(endAccountLoad - startAccountLoad);

    auto transactionSaveTime =
        std::chrono::duration_cast<
            std::chrono::milliseconds
        >(endTransactionSave - startTransactionSave);

    auto transactionLoadTime =
        std::chrono::duration_cast<
            std::chrono::milliseconds
        >(endTransactionLoad - startTransactionLoad);

    std::cout
        << "Accounts: "
        << accountCount
        << '\n';

    std::cout
        << "Transactions: "
        << transactionCount
        << '\n';

    std::cout
        << "Account save: "
        << accountSaveTime.count()
        << " milliseconds\n";

    std::cout
        << "Account load: "
        << accountLoadTime.count()
        << " milliseconds\n";

    std::cout
        << "Transaction save: "
        << transactionSaveTime.count()
        << " milliseconds\n";

    std::cout
        << "Transaction load: "
        << transactionLoadTime.count()
        << " milliseconds\n";

    std::cout
        << "Loaded accounts: "
        << loadedAccounts.size()
        << '\n';

    std::cout
        << "Loaded transactions: "
        << loadedTransactions.size()
        << '\n';

    return 0;
}