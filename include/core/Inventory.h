#pragma once

#include "models/Product.h"
#include "models/Warehouse.h"
#include "models/StockMovement.h"
#include "storage/Index.h"

#include <unordered_map>

class Inventory {
private:
    // warehouseId -> (productId -> quantity)
    std::unordered_map<
        int,
        std::unordered_map<int, int>
    > stock;

    Index<int, Product> products;

public:

    void addProduct(const Product& product);

    Product* getProduct(int productId);

    bool hasProduct(int productId) const;

    void purchase(
        int productId,
        int warehouseId,
        int quantity
    );

    bool sell(
        int productId,
        int warehouseId,
        int quantity
    );

    bool transfer(
        int productId,
        int fromWarehouseId,
        int toWarehouseId,
        int quantity
    );

    int getStock(
        int productId,
        int warehouseId
    ) const;
};