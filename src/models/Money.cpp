#include "models/Money.h"

Money::Money(std::int64_t paise)
    : paise(paise) {
}

std::int64_t Money::getPaise() const {
    return paise;
}

Money Money::operator+(const Money& other) const {
    return Money(paise + other.paise);
}

Money Money::operator-(const Money& other) const {
    return Money(paise - other.paise);
}

Money& Money::operator+=(const Money& other) {
    paise += other.paise;
    return *this;
}
    
Money& Money::operator-=(const Money& other) {
    paise -= other.paise;
    return *this;
}

bool Money::operator==(const Money& other) const {
    return paise == other.paise;
}

bool Money::operator!=(const Money& other) const {
    return !(*this == other);
}

bool Money::isZero() const {
    return paise == 0;
}

bool Money::isPositive() const {
    return paise > 0;
}