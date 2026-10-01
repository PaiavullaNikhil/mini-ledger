#include "core/Inventory.h"

#include <iostream>

int main() {

    Inventory inventory;

    Product laptop(
        101,
        "Laptop",
        Money(5000000)
    );

    Product mouse(
        102,
        "Mouse",
        Money(150000)
    );

    inventory.addProduct(laptop);
    inventory.addProduct(mouse);

    Product* product =
        inventory.getProduct(101);

    if (product != nullptr) {
        std::cout
            << "Found product: "
            << product->getName()
            << '\n';
    }

    inventory.purchase(
        101,
        1,
        10
    );

    inventory.purchase(
        102,
        1,
        20
    );

    std::cout
        << "Laptop stock: "
        << inventory.getStock(101, 1)
        << '\n';

    std::cout
        << "Mouse stock: "
        << inventory.getStock(102, 1)
        << '\n';

    bool sold = inventory.sell(
        101,
        1,
        2
    );

    std::cout
        << "Sale successful: "
        << std::boolalpha
        << sold
        << '\n';

    std::cout
        << "Laptop stock: "
        << inventory.getStock(101, 1)
        << '\n';

    bool invalidPurchase =
        inventory.sell(
            999,
            1,
            5
        );

    std::cout
        << "Unknown product sale: "
        << invalidPurchase
        << '\n';

    return 0;
}