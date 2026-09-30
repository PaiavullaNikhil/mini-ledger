#include "models/Account.h"

Account::Account(
    int id,
    const std::string& name,
    AccountType type
)
    : id(id),
      name(name),
      type(type),
      balance(Money(0)) {
}

int Account::getId() const {
    return id;
}

const std::string& Account::getName() const {
    return name;
}

AccountType Account::getType() const {
    return type;
}

Money Account::getBalance() const {
    return balance;
}

void Account::debit(const Money& amount) {

    if (type == AccountType::ASSET ||
        type == AccountType::EXPENSE) {

        balance += amount;

    } else {

        balance -= amount;
    }
}

void Account::credit(const Money& amount) {

    if (type == AccountType::ASSET ||
        type == AccountType::EXPENSE) {

        balance -= amount;

    } else {

        balance += amount;
    }
}

void Account::setBalanceForStorage(
    const Money& amount
) {
    balance = amount;
}