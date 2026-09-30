#pragma once

#include "models/Money.h"

#include <string>

class Product {
private:
    int id;
    std::string name;
    Money unitPrice;

public:
    Product(
        int id,
        const std::string& name,
        Money unitPrice
    );

    int getId() const;
    const std::string& getName() const;
    Money getUnitPrice() const;
};