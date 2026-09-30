#include "models/Warehouse.h"

Warehouse::Warehouse(
    int id,
    const std::string& name
)
    : id(id),
      name(name) {
}

int Warehouse::getId() const {
    return id;
}

const std::string& Warehouse::getName() const {
    return name;
}