#include "core/Inventory.h"

#include <thread>
#include <vector>
#include <atomic>
#include <gtest/gtest.h>

TEST(InventoryTest, AddsAndFindsProduct)
{
    Inventory inventory;

    Product product(
        101,
        "Laptop",
        Money(5000000)
    );

    inventory.addProduct(product);

    EXPECT_TRUE(
        inventory.hasProduct(101)
    );

    EXPECT_NE(
        inventory.getProduct(101),
        nullptr
    );

    EXPECT_EQ(
        inventory.getProduct(101)->getName(),
        "Laptop"
    );
}

TEST(InventoryTest, AddsAndFindsWarehouse)
{
    Inventory inventory;

    Warehouse warehouse(
        1,
        "Bangalore"
    );

    inventory.addWarehouse(warehouse);

    EXPECT_TRUE(
        inventory.hasWarehouse(1)
    );

    EXPECT_NE(
        inventory.getWarehouse(1),
        nullptr
    );

    EXPECT_EQ(
        inventory.getWarehouse(1)->getName(),
        "Bangalore"
    );
}

TEST(InventoryTest, PurchaseIncreasesStock)
{
    Inventory inventory;

    inventory.addProduct(
        Product(
            101,
            "Laptop",
            Money(5000000)
        )
    );

    inventory.addWarehouse(
        Warehouse(
            1,
            "Bangalore"
        )
    );

    inventory.purchase(
        101,
        1,
        10
    );

    EXPECT_EQ(
        inventory.getStock(101, 1),
        10
    );
}

TEST(InventoryTest, SaleDecreasesStock)
{
    Inventory inventory;

    inventory.addProduct(
        Product(
            101,
            "Laptop",
            Money(5000000)
        )
    );

    inventory.addWarehouse(
        Warehouse(
            1,
            "Bangalore"
        )
    );

    inventory.purchase(
        101,
        1,
        10
    );

    EXPECT_TRUE(
        inventory.sell(
            101,
            1,
            3
        )
    );

    EXPECT_EQ(
        inventory.getStock(101, 1),
        7
    );
}

TEST(InventoryTest, RejectsSaleWithInsufficientStock)
{
    Inventory inventory;

    inventory.addProduct(
        Product(
            101,
            "Laptop",
            Money(5000000)
        )
    );

    inventory.addWarehouse(
        Warehouse(
            1,
            "Bangalore"
        )
    );

    inventory.purchase(
        101,
        1,
        5
    );

    EXPECT_FALSE(
        inventory.sell(
            101,
            1,
            10
        )
    );

    EXPECT_EQ(
        inventory.getStock(101, 1),
        5
    );
}

TEST(InventoryTest, TransferMovesStock)
{
    Inventory inventory;

    inventory.addProduct(
        Product(
            101,
            "Laptop",
            Money(5000000)
        )
    );

    inventory.addWarehouse(
        Warehouse(
            1,
            "Bangalore"
        )
    );

    inventory.addWarehouse(
        Warehouse(
            2,
            "Mysore"
        )
    );

    inventory.purchase(
        101,
        1,
        10
    );

    EXPECT_TRUE(
        inventory.transfer(
            101,
            1,
            2,
            4
        )
    );

    EXPECT_EQ(
        inventory.getStock(101, 1),
        6
    );

    EXPECT_EQ(
        inventory.getStock(101, 2),
        4
    );
}

TEST(InventoryTest, RejectsTransferWithInsufficientStock)
{
    Inventory inventory;

    inventory.addProduct(
        Product(
            101,
            "Laptop",
            Money(5000000)
        )
    );

    inventory.addWarehouse(
        Warehouse(
            1,
            "Bangalore"
        )
    );

    inventory.addWarehouse(
        Warehouse(
            2,
            "Mysore"
        )
    );

    inventory.purchase(
        101,
        1,
        5
    );

    EXPECT_FALSE(
        inventory.transfer(
            101,
            1,
            2,
            10
        )
    );

    EXPECT_EQ(
        inventory.getStock(101, 1),
        5
    );

    EXPECT_EQ(
        inventory.getStock(101, 2),
        0
    );
}

TEST(InventoryTest, RejectsInvalidProduct)
{
    Inventory inventory;

    inventory.addWarehouse(
        Warehouse(
            1,
            "Bangalore"
        )
    );

    inventory.purchase(
        999,
        1,
        10
    );

    EXPECT_EQ(
        inventory.getStock(999, 1),
        0
    );
}

TEST(InventoryTest, RejectsInvalidWarehouse)
{
    Inventory inventory;

    inventory.addProduct(
        Product(
            101,
            "Laptop",
            Money(5000000)
        )
    );

    inventory.purchase(
        101,
        999,
        10
    );

    EXPECT_EQ(
        inventory.getStock(101, 999),
        0
    );
}

TEST(InventoryTest, RejectsInvalidQuantity)
{
    Inventory inventory;

    inventory.addProduct(
        Product(
            101,
            "Laptop",
            Money(5000000)
        )
    );

    inventory.addWarehouse(
        Warehouse(
            1,
            "Bangalore"
        )
    );

    inventory.purchase(
        101,
        1,
        -5
    );

    EXPECT_EQ(
        inventory.getStock(101, 1),
        0
    );

    EXPECT_FALSE(
        inventory.sell(
            101,
            1,
            0
        )
    );

    EXPECT_EQ(
        inventory.getStock(101, 1),
        0
    );
}

TEST(InventoryTest, HandlesConcurrentPurchases)
{
    Inventory inventory;

    inventory.addProduct(
        Product(
            1,
            "Laptop",
            Money(500000)
        )
    );

    inventory.addWarehouse(
        Warehouse(
            101,
            "Bangalore"
        )
    );

    const int threadCount = 10;
    const int purchasesPerThread = 100;
    const int quantityPerPurchase = 1;

    std::vector<std::thread> threads;

    for (int threadId = 0;
         threadId < threadCount;
         ++threadId)
    {
        threads.emplace_back(
            [&inventory]()
            {
                for (int i = 0;
                     i < purchasesPerThread;
                     ++i)
                {
                    inventory.purchase(
                        1,
                        101,
                        quantityPerPurchase
                    );
                }
            }
        );
    }

    for (auto &thread : threads)
    {
        thread.join();
    }

    EXPECT_EQ(
        inventory.getStock(1, 101),
        1000
    );
}

TEST(InventoryTest, HandlesConcurrentSales)
{
    Inventory inventory;

    inventory.addProduct(
        Product(
            1,
            "Laptop",
            Money(500000)
        )
    );

    inventory.addWarehouse(
        Warehouse(
            101,
            "Bangalore"
        )
    );

    inventory.purchase(
        1,
        101,
        1000
    );

    const int threadCount = 10;
    const int salesPerThread = 100;

    std::vector<std::thread> threads;
    std::atomic<int> successfulSales{0};

    for (int threadId = 0;
         threadId < threadCount;
         ++threadId)
    {
        threads.emplace_back(
            [&inventory, &successfulSales]()
            {
                for (int i = 0;
                     i < salesPerThread;
                     ++i)
                {
                    if (inventory.sell(
                            1,
                            101,
                            1))
                    {
                        ++successfulSales;
                    }
                }
            }
        );
    }

    for (auto &thread : threads)
    {
        thread.join();
    }

    EXPECT_EQ(
        successfulSales.load(),
        1000
    );

    EXPECT_EQ(
        inventory.getStock(1, 101),
        0
    );
}

TEST(InventoryTest, HandlesConcurrentTransfers)
{
    Inventory inventory;

    inventory.addProduct(
        Product(
            1,
            "Laptop",
            Money(500000)
        )
    );

    inventory.addWarehouse(
        Warehouse(
            101,
            "Bangalore"
        )
    );

    inventory.addWarehouse(
        Warehouse(
            102,
            "Mysore"
        )
    );

    inventory.purchase(
        1,
        101,
        1000
    );

    const int threadCount = 10;
    const int transfersPerThread = 100;

    std::vector<std::thread> threads;

    for (int threadId = 0;
         threadId < threadCount;
         ++threadId)
    {
        threads.emplace_back(
            [&inventory]()
            {
                for (int i = 0;
                     i < transfersPerThread;
                     ++i)
                {
                    inventory.transfer(
                        1,
                        101,
                        102,
                        1
                    );
                }
            }
        );
    }

    for (auto &thread : threads)
    {
        thread.join();
    }

    EXPECT_EQ(
        inventory.getStock(1, 101),
        0
    );

    EXPECT_EQ(
        inventory.getStock(1, 102),
        1000
    );
}