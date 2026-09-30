#pragma once

enum class StockMovementType {
    PURCHASE,
    SALE,
    TRANSFER_IN,
    TRANSFER_OUT
};

struct StockMovement {
    int productId;
    int warehouseId;
    int quantity;
    StockMovementType type;
};