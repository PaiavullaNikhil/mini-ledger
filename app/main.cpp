#include "core/Ledger.h"
#include "storage/StorageEngine.h"
#include "core/Inventory.h"

#include <iostream>

int main() {

    Inventory inventory;

    // Purchase 10 laptops in Bangalore
    inventory.purchase(
        101,
        1,
        10
    );

    std::cout
        << "Bangalore stock: "
        << inventory.getStock(101, 1)
        << '\n';

    // Sell 2 laptops
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
        << "Bangalore stock: "
        << inventory.getStock(101, 1)
        << '\n';

    // Transfer 3 laptops
    bool transferred = inventory.transfer(
        101,
        1,
        2,
        3
    );

    std::cout
        << "Transfer successful: "
        << transferred
        << '\n';

    std::cout
        << "Bangalore stock: "
        << inventory.getStock(101, 1)
        << '\n';

    std::cout
        << "Mysore stock: "
        << inventory.getStock(101, 2)
        << '\n';

    // Try to sell more than available
    bool failedSale = inventory.sell(
        101,
        1,
        10
    );

    std::cout
        << "Invalid sale successful: "
        << failedSale
        << '\n';

    return 0;
}