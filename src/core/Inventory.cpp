#include "core/Inventory.h"

Inventory::Inventory(
    AuditLog *auditLog)
    : auditLog(auditLog)
{
}

void Inventory::addProduct(
    const Product &product)
{
    products.insert(
        product.getId(),
        product);
}

Product *Inventory::getProduct(
    int productId)
{
    return products.find(productId);
}

std::vector<Product> Inventory::getProducts() const
{
    return products.values();
}

bool Inventory::hasProduct(
    int productId) const
{
    return products.contains(productId);
}

void Inventory::purchase(
    int productId,
    int warehouseId,
    int quantity)
{
    if (!hasProduct(productId) ||
        !hasWarehouse(warehouseId) ||
        quantity <= 0)
    {
        return;
    }

    if (auditLog != nullptr)
    {
        auditLog->log(
            "PURCHASE | Product=" +
            std::to_string(productId) +
            " | Warehouse=" +
            std::to_string(warehouseId) +
            " | Qty=" +
            std::to_string(quantity));
    }

    stock[warehouseId][productId] += quantity;
}

bool Inventory::sell(
    int productId,
    int warehouseId,
    int quantity)
{
    if (!hasProduct(productId) ||
        !hasWarehouse(warehouseId) ||
        quantity <= 0)
    {
        return false;
    }

    int currentStock =
        getStock(productId, warehouseId);

    if (currentStock < quantity)
    {
        return false;
    }

    stock[warehouseId][productId] -= quantity;

    if (auditLog != nullptr)
    {
        auditLog->log(
            "SALE | Product=" +
            std::to_string(productId) +
            " | Warehouse=" +
            std::to_string(warehouseId) +
            " | Qty=" +
            std::to_string(quantity));
    }

    return true;
}

bool Inventory::transfer(
    int productId,
    int fromWarehouseId,
    int toWarehouseId,
    int quantity)
{
    if (!hasProduct(productId) ||
        !hasWarehouse(fromWarehouseId) ||
        !hasWarehouse(toWarehouseId) ||
        quantity <= 0)
    {
        return false;
    }

    int currentStock =
        getStock(productId, fromWarehouseId);

    if (currentStock < quantity)
    {
        return false;
    }

    stock[fromWarehouseId][productId] -= quantity;
    stock[toWarehouseId][productId] += quantity;

    if (auditLog != nullptr)
    {
        auditLog->log(
            "TRANSFER | Product=" +
            std::to_string(productId) +
            " | From=" +
            std::to_string(fromWarehouseId) +
            " | To=" +
            std::to_string(toWarehouseId) +
            " | Qty=" +
            std::to_string(quantity));
    }

    return true;
}

int Inventory::getStock(
    int productId,
    int warehouseId) const
{
    auto warehouseIt =
        stock.find(warehouseId);

    if (warehouseIt == stock.end())
    {
        return 0;
    }

    auto productIt =
        warehouseIt->second.find(productId);

    if (productIt == warehouseIt->second.end())
    {
        return 0;
    }

    return productIt->second;
}

void Inventory::addWarehouse(
    const Warehouse &warehouse)
{
    warehouses.insert(
        warehouse.getId(),
        warehouse);
}

Warehouse *Inventory::getWarehouse(
    int warehouseId)
{
    return warehouses.find(warehouseId);
}

bool Inventory::hasWarehouse(
    int warehouseId) const
{
    return warehouses.contains(warehouseId);
}

std::vector<Warehouse> Inventory::getWarehouses() const
{
    return warehouses.values();
}