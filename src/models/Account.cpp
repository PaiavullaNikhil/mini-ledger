#include "models/Account.h"

Account::Account(int id, const std::string& name)
    : id(id), name(name), balance(0.0) {
}

int Account::getId() const {
    return id;
}

const std::string& Account::getName() const {
    return name;
}

double Account::getBalance() const {
    return balance;
}

void Account::debit(double amount) {
    balance += amount;
}

void Account::credit(double amount) {
    balance -= amount;
}