#pragma once

#include "models/Transaction.h"

#include <string>
#include <vector>

class WAL {
private:
    std::string filePath;

public:
    explicit WAL(
        const std::string& filePath
    );

    void append(
        const Transaction& transaction
    ) const;

    std::vector<Transaction> read() const;

    void clear() const;
};