#include "models/Product.h"
#include "storage/Index.h"

#include <chrono>
#include <iostream>
#include <vector>

int main() {

    const int count = 1'000'000;

    std::vector<Product> products;
    products.reserve(count);

    Index<int, Product> index;

    for (int i = 0; i < count; ++i) {

        Product product(
            i,
            "Product",
            Money(10000)
        );

        products.push_back(product);

        index.insert(
            product.getId(),
            product
        );
    }

    int targetId = count - 1;

    auto startLinear =
        std::chrono::high_resolution_clock::now();

    Product* linearResult = nullptr;

    for (auto& product : products) {
        if (product.getId() == targetId) {
            linearResult = &product;
            break;
        }
    }

    auto endLinear =
        std::chrono::high_resolution_clock::now();

    auto startIndexed =
        std::chrono::high_resolution_clock::now();

    Product* indexedResult =
        index.find(targetId);

    auto endIndexed =
        std::chrono::high_resolution_clock::now();

    auto linearTime =
        std::chrono::duration_cast<
            std::chrono::microseconds
        >(endLinear - startLinear);

    auto indexedTime =
        std::chrono::duration_cast<
            std::chrono::microseconds
        >(endIndexed - startIndexed);

    std::cout
        << "Linear search: "
        << linearTime.count()
        << " microseconds\n";

    std::cout
        << "Indexed search: "
        << indexedTime.count()
        << " microseconds\n";

    std::cout
        << "Linear found: "
        << (linearResult != nullptr)
        << '\n';

    std::cout
        << "Indexed found: "
        << (indexedResult != nullptr)
        << '\n';

    return 0;
}