#include "core/Ledger.h"

#include <iostream>

int main() {

    Ledger ledger;

    Account cash(1001, "Cash");
    Account capital(1002, "Capital");

    ledger.addAccount(cash);
    ledger.addAccount(capital);

    Transaction transaction(
        5001,
        "Initial capital investment"
    );

    transaction.addEntry(
        1001,
        100000,
        0
    );

    transaction.addEntry(
        1002,
        0,
        90000
    );

    if (ledger.postTransaction(transaction)) {
        std::cout << "Transaction posted successfully.\n";
    } else {
        std::cout << "Transaction failed.\n";
    }

    std::cout << "\nAccount Balances:\n";

    ledger.printBalances();

    return 0;
}