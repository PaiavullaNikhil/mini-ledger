#pragma once

#include "core/Ledger.h"
#include "core/Inventory.h"

class ReportEngine {
private:
    const Ledger& ledger;
    const Inventory& inventory;

public:
    explicit ReportEngine(
        const Ledger& ledger,
        const Inventory& inventory
    );

    void printTrialBalance() const;

    void printProfitAndLoss() const;

    void printBalanceSheet() const;

    void printAccountLedger(
        int accountId
    ) const;

    void printInventoryReport() const;
    
};