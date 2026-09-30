#include "models/Product.h"

Product::Product(
    int id,
    const std::string& name,
    Money unitPrice
)
    : id(id),
      name(name),
      unitPrice(unitPrice) {
}

int Product::getId() const {
    return id;
}

const std::string& Product::getName() const {
    return name;
}

Money Product::getUnitPrice() const {
    return unitPrice;
}