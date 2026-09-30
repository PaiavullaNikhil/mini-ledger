#include "core/Ledger.h"
#include "storage/StorageEngine.h"

#include <iostream>

int main() {

    StorageEngine storage("data");

    auto loadedAccounts = storage.loadAccounts();

    if (!loadedAccounts.empty()) {

        std::cout << "Loaded accounts from disk:\n\n";

        for (const auto& account : loadedAccounts) {

            std::cout
                << account.getId()
                << " | "
                << account.getName()
                << " | "
                << account.getBalance().getPaise()
                << " paise\n";
        }

        return 0;
    }

    std::cout << "No existing accounts found.\n";
    std::cout << "Creating initial accounts...\n\n";

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

    storage.saveAccounts(
        ledger.getAccounts()
    );

    std::cout
        << "Accounts saved successfully.\n";

    return 0;
}