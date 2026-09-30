#pragma once

#include "models/Account.h"
#include "models/Transaction.h"

#include <fstream>

class Serializer {
public:

    static void writeAccount(
        std::ofstream& out,
        const Account& account
    );

    static Account readAccount(
        std::ifstream& in
    );

    static void writeTransaction(
        std::ofstream& out,
        const Transaction& transaction
    );

    static Transaction readTransaction(
        std::ifstream& in
    );
};