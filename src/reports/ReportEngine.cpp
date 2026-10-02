#include "reports/ReportEngine.h"

#include <cstdint>
#include <iostream>

ReportEngine::ReportEngine(
    const Ledger& ledger,
    const Inventory& inventory
)
    : ledger(ledger),
      inventory(inventory) {
}

void ReportEngine::printTrialBalance() const {

    std::cout << "\n===== TRIAL BALANCE =====\n";

    std::int64_t totalDebit = 0;
    std::int64_t totalCredit = 0;

    for (const auto& account :
         ledger.getAccounts()) {

        std::int64_t balance =
            account.getBalance().getPaise();

        bool debitNormal =
            account.getType() == AccountType::ASSET ||
            account.getType() == AccountType::EXPENSE;

        std::int64_t debit = 0;
        std::int64_t credit = 0;

        if (debitNormal) {

            if (balance >= 0) {
                debit = balance;
            } else {
                credit = -balance;
            }

        } else {

            if (balance >= 0) {
                credit = balance;
            } else {
                debit = -balance;
            }
        }

        totalDebit += debit;
        totalCredit += credit;

        std::cout
            << account.getId()
            << " | "
            << account.getName()
            << " | Debit: "
            << debit
            << " | Credit: "
            << credit
            << '\n';
    }

    std::cout
        << "-------------------------\n";

    std::cout
        << "Total Debit: "
        << totalDebit
        << '\n';

    std::cout
        << "Total Credit: "
        << totalCredit
        << '\n';
}

void ReportEngine::printProfitAndLoss() const {

    std::cout << "\n===== PROFIT & LOSS =====\n";

    std::int64_t totalRevenue = 0;
    std::int64_t totalExpenses = 0;

    for (const auto& account :
         ledger.getAccounts()) {

        std::int64_t balance =
            account.getBalance().getPaise();

        if (account.getType() ==
            AccountType::REVENUE) {

            totalRevenue += balance;

            std::cout
                << "Revenue | "
                << account.getName()
                << " | "
                << balance
                << '\n';
        }

        if (account.getType() ==
            AccountType::EXPENSE) {

            totalExpenses += balance;

            std::cout
                << "Expense | "
                << account.getName()
                << " | "
                << balance
                << '\n';
        }
    }

    std::int64_t netProfit =
        totalRevenue - totalExpenses;

    std::cout
        << "-------------------------\n";

    std::cout
        << "Total Revenue: "
        << totalRevenue
        << '\n';

    std::cout
        << "Total Expenses: "
        << totalExpenses
        << '\n';

    std::cout
        << "Net Profit: "
        << netProfit
        << '\n';
}

void ReportEngine::printBalanceSheet() const {

    std::cout << "\n===== BALANCE SHEET =====\n";

    std::int64_t totalAssets = 0;
    std::int64_t totalLiabilities = 0;
    std::int64_t totalEquity = 0;
    std::int64_t totalRevenue = 0;
    std::int64_t totalExpenses = 0;

    for (const auto& account :
         ledger.getAccounts()) {

        std::int64_t balance =
            account.getBalance().getPaise();

        if (account.getType() ==
            AccountType::ASSET) {

            totalAssets += balance;

            std::cout
                << "Asset | "
                << account.getName()
                << " | "
                << balance
                << '\n';
        }

        else if (account.getType() ==
                 AccountType::LIABILITY) {

            totalLiabilities += balance;

            std::cout
                << "Liability | "
                << account.getName()
                << " | "
                << balance
                << '\n';
        }

        else if (account.getType() ==
                 AccountType::EQUITY) {

            totalEquity += balance;

            std::cout
                << "Equity | "
                << account.getName()
                << " | "
                << balance
                << '\n';
        }

        else if (account.getType() ==
                 AccountType::REVENUE) {

            totalRevenue += balance;
        }

        else if (account.getType() ==
                 AccountType::EXPENSE) {

            totalExpenses += balance;
        }
    }

    std::int64_t netProfit =
        totalRevenue - totalExpenses;

    totalEquity += netProfit;

    std::int64_t totalLiabilitiesAndEquity =
        totalLiabilities + totalEquity;

    std::cout
        << "Current Profit: "
        << netProfit
        << '\n';

    std::cout
        << "-------------------------\n";

    std::cout
        << "Total Assets: "
        << totalAssets
        << '\n';

    std::cout
        << "Total Liabilities: "
        << totalLiabilities
        << '\n';

    std::cout
        << "Total Equity: "
        << totalEquity
        << '\n';

    std::cout
        << "Liabilities + Equity: "
        << totalLiabilitiesAndEquity
        << '\n';

    std::cout
        << "Balanced: "
        << std::boolalpha
        << (totalAssets ==
            totalLiabilitiesAndEquity)
        << '\n';
}

void ReportEngine::printAccountLedger(
    int accountId
) const {

    const Account* account =
        ledger.getAccount(accountId);

    if (account == nullptr) {
        std::cout
            << "Account not found.\n";
        return;
    }

    std::cout
        << "\n===== ACCOUNT LEDGER: "
        << account->getName()
        << " =====\n";

    std::int64_t balance = 0;

    for (const auto& transaction :
         ledger.getTransactions()) {

        for (const auto& entry :
             transaction.getEntries()) {

            if (entry.accountId != accountId) {
                continue;
            }

            std::int64_t debit =
                entry.debit.getPaise();

            std::int64_t credit =
                entry.credit.getPaise();

            std::cout
                << "Transaction "
                << transaction.getId()
                << " | "
                << transaction.getDescription()
                << '\n';

            std::cout
                << "Debit: "
                << debit
                << " | Credit: "
                << credit
                << '\n';

            balance += debit;
            balance -= credit;
        }
    }

    std::cout
        << "-------------------------\n";

    std::cout
        << "Closing Balance: "
        << balance
        << '\n';
}

void ReportEngine::printInventoryReport() const
{
    std::cout << "\n===== INVENTORY REPORT =====\n";

    for (const auto& product : inventory.getProducts())
    {
        std::cout
            << "Product: "
            << product.getName()
            << " | ID: "
            << product.getId()
            << '\n';

        for (const auto& warehouse :
             inventory.getWarehouses())
        {
            int quantity =
                inventory.getStock(
                    product.getId(),
                    warehouse.getId());

            if (quantity > 0)
            {
                std::cout
                    << "  Warehouse: "
                    << warehouse.getName()
                    << " | Quantity: "
                    << quantity
                    << '\n';
            }
        }
    }
}