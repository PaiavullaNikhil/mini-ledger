#include "core/Ledger.h"
#include "models/Account.h"
#include "models/Transaction.h"
#include "models/Money.h"

#include <chrono>
#include <iostream>

int main()
{
    const int count = 10'000;

    Ledger ledger;

    ledger.addAccount(
        Account(
            1,
            "Cash",
            AccountType::ASSET
        )
    );

    ledger.addAccount(
        Account(
            2,
            "Capital",
            AccountType::EQUITY
        )
    );

    auto start =
        std::chrono::high_resolution_clock::now();

    for (int i = 0; i < count; ++i)
    {
        Transaction transaction(
            i,
            "Benchmark transaction"
        );

        transaction.addEntry(
            1,
            Money(100),
            Money(0)
        );

        transaction.addEntry(
            2,
            Money(0),
            Money(100)
        );

        ledger.postTransaction(transaction);
    }

    auto end =
        std::chrono::high_resolution_clock::now();

    auto elapsed =
        std::chrono::duration_cast<
            std::chrono::milliseconds
        >(end - start);

    std::cout
        << "Transactions: "
        << count
        << '\n';

    std::cout
        << "Total time: "
        << elapsed.count()
        << " milliseconds\n";

    double transactionsPerSecond =
        count /
        (elapsed.count() / 1000.0);

    std::cout
        << "Transactions/second: "
        << transactionsPerSecond
        << '\n';

    std::cout
        << "Stored transactions: "
        << ledger.getTransactions().size()
        << '\n';

    return 0;
}