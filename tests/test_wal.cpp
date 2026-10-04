#include "storage/WAL.h"

#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <iterator>
#include <vector>

namespace fs = std::filesystem;

class WALTest : public ::testing::Test
{
protected:
    const std::string walPath =
        "data/test_wal.log";

    void SetUp() override
    {
        fs::remove(walPath);
    }

    void TearDown() override
    {
        fs::remove(walPath);
    }
};

TEST_F(WALTest, AppendsAndReadsTransaction)
{
    WAL wal(walPath);

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

    wal.append(transaction);

    std::vector<Transaction> loaded =
        wal.read();

    ASSERT_EQ(loaded.size(), 1);

    EXPECT_EQ(
        loaded[0].getId(),
        5001
    );

    EXPECT_EQ(
        loaded[0].getDescription(),
        "Cash sale"
    );

    EXPECT_EQ(
        loaded[0].getEntries().size(),
        2
    );
}

TEST_F(WALTest, ReadsMultipleTransactions)
{
    WAL wal(walPath);

    Transaction first(
        5001,
        "First"
    );

    first.addEntry(
        1001,
        Money(1000),
        Money(0)
    );

    first.addEntry(
        1002,
        Money(0),
        Money(1000)
    );

    Transaction second(
        5002,
        "Second"
    );

    second.addEntry(
        1001,
        Money(2000),
        Money(0)
    );

    second.addEntry(
        1002,
        Money(0),
        Money(2000)
    );

    wal.append(first);
    wal.append(second);

    std::vector<Transaction> loaded =
        wal.read();

    ASSERT_EQ(loaded.size(), 2);

    EXPECT_EQ(
        loaded[0].getId(),
        5001
    );

    EXPECT_EQ(
        loaded[1].getId(),
        5002
    );
}

TEST_F(WALTest, ClearRemovesAllTransactions)
{
    WAL wal(walPath);

    Transaction transaction(
        5001,
        "Test"
    );

    transaction.addEntry(
        1001,
        Money(1000),
        Money(0)
    );

    transaction.addEntry(
        1002,
        Money(0),
        Money(1000)
    );

    wal.append(transaction);

    ASSERT_FALSE(
        wal.read().empty()
    );

    wal.clear();

    EXPECT_TRUE(
        wal.read().empty()
    );
}

TEST_F(WALTest, IncompleteFinalRecordIsIgnored)
{
    WAL wal(walPath);

    Transaction transaction(
        5001,
        "Test"
    );

    transaction.addEntry(
        1001,
        Money(1000),
        Money(0)
    );

    transaction.addEntry(
        1002,
        Money(0),
        Money(1000)
    );

    wal.append(transaction);

    std::ifstream input(
        walPath,
        std::ios::binary
    );

    const std::istreambuf_iterator<char> inputBegin(input);
    const std::istreambuf_iterator<char> inputEnd;
    std::vector<char> bytes(inputBegin, inputEnd);

    input.close();

    ASSERT_GT(bytes.size(), 5);

    bytes.resize(
        bytes.size() - 5
    );

    std::ofstream output(
        walPath,
        std::ios::binary |
        std::ios::trunc
    );

    output.write(
        bytes.data(),
        static_cast<std::streamsize>(
            bytes.size()
        )
    );

    output.close();

    EXPECT_NO_THROW(
        wal.read()
    );
}