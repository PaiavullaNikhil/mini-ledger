#pragma once

#include "models/Product.h"
#include "models/Warehouse.h"
#include "models/StockMovement.h"
#include "storage/Index.h"
#include "audit/AuditLog.h"

#include <unordered_map>
#include <vector>

class Inventory
{
private:
    // warehouseId -> (productId -> quantity)
    std::unordered_map<int, std::unordered_map<int, int>> stock;

    Index<int, Product> products;
    Index<int, Warehouse> warehouses;

    AuditLog *auditLog;

public:
    explicit Inventory(
        AuditLog *auditLog = nullptr);

    void addProduct(const Product &product);

    Product *getProduct(int productId);

    std::vector<Product> getProducts() const;

    bool hasProduct(int productId) const;

    void purchase(int productId,int warehouseId,int quantity);

    bool sell(int productId,int warehouseId,int quantity);

    bool transfer(int productId,int fromWarehouseId,int toWarehouseId,int quantity);

    int getStock(int productId, int warehouseId) const;

    void addWarehouse(const Warehouse &warehouse);

    Warehouse *getWarehouse(int warehouseId);

    bool hasWarehouse(int warehouseId) const;

    std::vector<Warehouse> getWarehouses() const;
};