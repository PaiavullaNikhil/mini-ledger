#include "storage/Index.h"

#include <gtest/gtest.h>
#include <algorithm>
#include "storage/Index.h"

TEST(IndexTest, InsertsAndFindsValue)
{
    Index<int, std::string> index;

    index.insert(1, "Laptop");

    const auto *value = index.find(1);

    ASSERT_NE(value, nullptr);
    EXPECT_EQ(*value, "Laptop");
}

TEST(IndexTest, ReturnsNullForMissingKey)
{
    Index<int, std::string> index;

    EXPECT_EQ(index.find(999), nullptr);
}

TEST(IndexTest, ChecksIfKeyExists)
{
    Index<int, std::string> index;

    index.insert(1, "Laptop");

    EXPECT_TRUE(index.contains(1));
    EXPECT_FALSE(index.contains(999));
}

TEST(IndexTest, UpdatesExistingValue)
{
    Index<int, std::string> index;

    index.insert(1, "Laptop");
    index.insert(1, "Gaming Laptop");

    const auto *value = index.find(1);

    ASSERT_NE(value, nullptr);
    EXPECT_EQ(*value, "Gaming Laptop");
}

TEST(IndexTest, ErasesValue)
{
    Index<int, std::string> index;

    index.insert(1, "Laptop");

    EXPECT_TRUE(index.erase(1));
    EXPECT_FALSE(index.contains(1));
}

TEST(IndexTest, EraseReturnsFalseForMissingKey)
{
    Index<int, std::string> index;

    EXPECT_FALSE(index.erase(999));
}

TEST(IndexTest, TracksSize)
{
    Index<int, std::string> index;

    EXPECT_EQ(index.size(), 0);

    index.insert(1, "Laptop");
    index.insert(2, "Mouse");

    EXPECT_EQ(index.size(), 2);

    index.erase(1);

    EXPECT_EQ(index.size(), 1);
}

TEST(IndexTest, ReturnsAllValues)
{
    Index<int, std::string> index;

    index.insert(1, "Laptop");
    index.insert(2, "Mouse");

    auto values = index.values();

    ASSERT_EQ(values.size(), 2);

    EXPECT_TRUE(
        std::find(values.begin(), values.end(), "Laptop")
        != values.end()
    );

    EXPECT_TRUE(
        std::find(values.begin(), values.end(), "Mouse")
        != values.end()
    );
}