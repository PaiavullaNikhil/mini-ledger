#include "audit/AuditLog.h"
#include "core/Inventory.h"
#include "core/Ledger.h"
#include "models/Account.h"
#include "models/Product.h"
#include "models/Warehouse.h"
#include "models/Transaction.h"
#include "reports/ReportEngine.h"
#include "storage/StorageEngine.h"
#include "storage/WAL.h"

#include <iostream>

int main()
{
    StorageEngine storage("data");

    AuditLog auditLog(
        "data/audit.log");

    WAL wal(
        "data/wal.log");

    Ledger ledger(
        &auditLog,
        &wal);

    std::vector<Account> storedAccounts =
        storage.loadAccounts();

    std::vector<Transaction> storedTransactions =
        storage.loadTransactions();

    if (storedAccounts.empty())
    {
        Account cash(
            1001,
            "Cash",
            AccountType::ASSET);

        Account capital(
            1002,
            "Capital",
            AccountType::EQUITY);

        Account sales(
            1003,
            "Sales",
            AccountType::REVENUE);

        Account salary(
            1004,
            "Salary Expense",
            AccountType::EXPENSE);

        ledger.addAccount(cash);
        ledger.addAccount(capital);
        ledger.addAccount(sales);
        ledger.addAccount(salary);

        Transaction investment(
            5001,
            "Initial capital");

        investment.addEntry(
            1001,
            Money(10000000),
            Money(0));

        investment.addEntry(
            1002,
            Money(0),
            Money(10000000));

        ledger.postTransaction(
            investment);

        Transaction sale(
            5002,
            "Cash sale");

        sale.addEntry(
            1001,
            Money(2000000),
            Money(0));

        sale.addEntry(
            1003,
            Money(0),
            Money(2000000));

        ledger.postTransaction(
            sale);

        Transaction salaryPayment(
            5003,
            "Salary payment");

        salaryPayment.addEntry(
            1004,
            Money(500000),
            Money(0));

        salaryPayment.addEntry(
            1001,
            Money(0),
            Money(500000));

        ledger.postTransaction(
            salaryPayment);

        storage.saveAccounts(
            ledger.getAccounts());

        storage.saveTransactions(
            ledger.getTransactions());
    }
    else
    {
        ledger.loadAccounts(
            storedAccounts);

        ledger.loadTransactions(
            storedTransactions);

        ledger.recover();

        storage.saveAccounts(
            ledger.getAccounts());

        storage.saveTransactions(
            ledger.getTransactions());

        wal.clear();
    }

    Inventory inventory(
        &auditLog);

    Product laptop(
        101,
        "Laptop",
        Money(5000000));

    Product mouse(
        102,
        "Mouse",
        Money(150000));

    Warehouse bangalore(
        1,
        "Bangalore");

    Warehouse mysore(
        2,
        "Mysore");

    inventory.addProduct(
        laptop);

    inventory.addProduct(
        mouse);

    inventory.addWarehouse(
        bangalore);

    inventory.addWarehouse(
        mysore);

    inventory.purchase(
        101,
        1,
        10);

    inventory.sell(
        101,
        1,
        2);

    inventory.transfer(
        101,
        1,
        2,
        3);

    inventory.purchase(
        102,
        1,
        20);

    ReportEngine reports(
        ledger,
        inventory);

    reports.printTrialBalance();

    reports.printProfitAndLoss();

    reports.printBalanceSheet();

    reports.printAccountLedger(
        1001);

    reports.printInventoryReport();

    return 0;
}