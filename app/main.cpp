#include "core/Ledger.h"
#include "storage/StorageEngine.h"

#include <iostream>

int main() {

    StorageEngine storage("data");

    Ledger ledger;

    Account cash(
        1001,
        "Cash",
        AccountType::ASSET
    );

    Account capital(
        1002,
        "Capital",
        AccountType::EQUITY
    );

    ledger.addAccount(cash);
    ledger.addAccount(capital);

    Transaction investment(
        5001,
        "Initial capital investment"
    );

    investment.addEntry(
        1001,
        Money(10000000),
        Money(0)
    );

    investment.addEntry(
        1002,
        Money(0),
        Money(10000000)
    );

    if (!ledger.postTransaction(investment)) {

        std::cout
            << "Failed to post transaction.\n";

        return 1;
    }

    // Save
    storage.saveAccounts(
        ledger.getAccounts()
    );

    storage.saveTransactions(
        ledger.getTransactions()
    );

    std::cout << "Data saved.\n\n";


    // Load
    auto loadedAccounts =
        storage.loadAccounts();

    auto loadedTransactions =
        storage.loadTransactions();


    std::cout << "Loaded Accounts:\n";

    for (const auto& account :
         loadedAccounts) {

        std::cout
            << account.getId()
            << " | "
            << account.getName()
            << " | "
            << account.getBalance().getPaise()
            << " paise\n";
    }


    std::cout << "\nLoaded Transactions:\n";

    for (const auto& transaction :
         loadedTransactions) {

        std::cout
            << "Transaction #"
            << transaction.getId()
            << " | "
            << transaction.getDescription()
            << '\n';

        for (const auto& entry :
             transaction.getEntries()) {

            std::cout
                << "  Account: "
                << entry.accountId
                << " | Debit: "
                << entry.debit.getPaise()
                << " | Credit: "
                << entry.credit.getPaise()
                << '\n';
        }
    }

    return 0;
}