#include "core/Inventory.h"

void Inventory::purchase(
    int productId,
    int warehouseId,
    int quantity
) {
    if (quantity <= 0) {
        return;
    }

    stock[warehouseId][productId] += quantity;
}

bool Inventory::sell(
    int productId,
    int warehouseId,
    int quantity
) {
    if (quantity <= 0) {
        return false;
    }

    int currentStock =
        getStock(productId, warehouseId);

    if (currentStock < quantity) {
        return false;
    }

    stock[warehouseId][productId] -= quantity;

    return true;
}

bool Inventory::transfer(
    int productId,
    int fromWarehouseId,
    int toWarehouseId,
    int quantity
) {
    if (quantity <= 0) {
        return false;
    }

    int currentStock =
        getStock(productId, fromWarehouseId);

    if (currentStock < quantity) {
        return false;
    }

    stock[fromWarehouseId][productId] -= quantity;

    stock[toWarehouseId][productId] += quantity;

    return true;
}

int Inventory::getStock(
    int productId,
    int warehouseId
) const {
    auto warehouseIt =
        stock.find(warehouseId);

    if (warehouseIt == stock.end()) {
        return 0;
    }

    auto productIt =
        warehouseIt->second.find(productId);

    if (productIt == warehouseIt->second.end()) {
        return 0;
    }

    return productIt->second;
}