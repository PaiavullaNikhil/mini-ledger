#pragma once

#include "models/Account.h"

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
};