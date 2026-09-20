#pragma once

#include <string>

class Account {
private:
    int id;
    std::string name;
    double balance;

public:
    Account(int id, const std::string& name);

    int getId() const;
    const std::string& getName() const;
    double getBalance() const;

    void debit(double amount);  //withdraw
    void credit(double amount); //add
};