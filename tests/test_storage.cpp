#include "storage/StorageEngine.h"

#include <gtest/gtest.h>

#include <filesystem>

namespace fs = std::filesystem;

class StorageTest : public ::testing::Test
{
protected:
    const std::string testDirectory =
        "data/test_storage";

    void SetUp() override
    {
        fs::remove_all(testDirectory);
    }

    void TearDown() override
    {
        fs::remove_all(testDirectory);
    }
};

TEST_F(StorageTest, SavesAndLoadsAccounts)
{
    StorageEngine storage(testDirectory);

    std::vector<Account> accounts;

    accounts.emplace_back(
        1001,
        "Cash",
        AccountType::ASSET
    );

    accounts[0].debit(Money(10000));

    storage.saveAccounts(accounts);

    std::vector<Account> loaded =
        storage.loadAccounts();

    ASSERT_EQ(loaded.size(), 1);

    EXPECT_EQ(loaded[0].getId(), 1001);
    EXPECT_EQ(loaded[0].getName(), "Cash");
    EXPECT_EQ(
        loaded[0].getBalance().getPaise(),
        10000
    );
}

TEST_F(StorageTest, SavesAndLoadsTransactions)
{
    StorageEngine storage(testDirectory);

    Transaction transaction(
        5001,
        "Cash sale"
    );

    transaction.addEntry(
        1001,
        Money(10000),
        Money(0)
    );

    transaction.addEntry(
        1003,
        Money(0),
        Money(10000)
    );

    storage.saveTransactions(
        {transaction}
    );

    std::vector<Transaction> loaded =
        storage.loadTransactions();

    ASSERT_EQ(loaded.size(), 1);

    EXPECT_EQ(
        loaded[0].getId(),
        5001
    );

    EXPECT_EQ(
        loaded[0].getDescription(),
        "Cash sale"
    );

    ASSERT_EQ(
        loaded[0].getEntries().size(),
        2
    );

    EXPECT_EQ(
        loaded[0].getEntries()[0].debit.getPaise(),
        10000
    );
}

TEST_F(StorageTest, EmptyStorageReturnsEmptyVectors)
{
    StorageEngine storage(testDirectory);

    EXPECT_TRUE(
        storage.loadAccounts().empty()
    );

    EXPECT_TRUE(
        storage.loadTransactions().empty()
    );
}