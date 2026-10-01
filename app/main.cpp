#include "audit/AuditLog.h"
#include "core/Inventory.h"
#include "core/Ledger.h"

#include <iostream>

int main() {

    AuditLog auditLog(
        "data/audit.log"
    );

    Ledger ledger(&auditLog);
    Inventory inventory(&auditLog);

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

    Transaction transaction(
        5001,
        "Initial capital investment"
    );

    transaction.addEntry(
        1001,
        Money(10000000),
        Money(0)
    );

    transaction.addEntry(
        1002,
        Money(0),
        Money(10000000)
    );

    ledger.postTransaction(transaction);

    Product laptop(
        101,
        "Laptop",
        Money(5000000)
    );

    inventory.addProduct(laptop);

    inventory.purchase(
        101,
        1,
        10
    );

    inventory.sell(
        101,
        1,
        2
    );

    inventory.transfer(
        101,
        1,
        2,
        3
    );

    std::cout
        << "Operations completed.\n";

    return 0;
}