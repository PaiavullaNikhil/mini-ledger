#pragma once

#include <cstdint>

class Money {
private:
	std::int64_t paise;

public:
	explicit Money(std::int64_t paise = 0);

	std::int64_t getPaise() const;

	Money operator+(const Money& other) const;
	Money operator-(const Money& other) const;
	Money& operator+=(const Money& other);
	Money& operator-=(const Money& other);

	bool operator==(const Money& other) const;
	bool operator!=(const Money& other) const;

	bool isZero() const;
	bool isPositive() const;
};
