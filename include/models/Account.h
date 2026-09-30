#pragma once

#include "models/AccountType.h"
#include "models/Money.h"

#include <string>

class Account {
private:
    int id;
    std::string name;
    AccountType type;
    Money balance;

public:
    Account(
        int id,
        const std::string& name,
        AccountType type
    );

    int getId() const;
    const std::string& getName() const;
    AccountType getType() const;
    Money getBalance() const;

    void debit(const Money& amount);
    void credit(const Money& amount);
    
    void setBalanceForStorage(const Money& amount);
};