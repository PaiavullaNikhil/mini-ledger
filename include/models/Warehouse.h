#pragma once

#include <string>

class Warehouse {
private:
    int id;
    std::string name;

public:
    Warehouse(
        int id,
        const std::string& name
    );

    int getId() const;
    const std::string& getName() const;
};