#include "models/Transaction.h"

#include <gtest/gtest.h>

TEST(TransactionTest, BalancedTransaction)
{
    Transaction transaction(
        5001,
        "Test transaction"
    );

    transaction.addEntry(
        1001,
        Money(10000),
        Money(0)
    );

    transaction.addEntry(
        1002,
        Money(0),
        Money(10000)
    );

    EXPECT_TRUE(
        transaction.isBalanced()
    );
}

TEST(TransactionTest, UnbalancedTransaction)
{
    Transaction transaction(
        5002,
        "Unbalanced transaction"
    );

    transaction.addEntry(
        1001,
        Money(10000),
        Money(0)
    );

    transaction.addEntry(
        1002,
        Money(0),
        Money(5000)
    );

    EXPECT_FALSE(
        transaction.isBalanced()
    );
}

TEST(TransactionTest, RejectsBothDebitAndCredit)
{
    Transaction transaction(
        5003,
        "Invalid transaction"
    );

    EXPECT_THROW(
        transaction.addEntry(
            1001,
            Money(10000),
            Money(5000)
        ),
        std::invalid_argument
    );
}

TEST(TransactionTest, RejectsZeroDebitAndCredit)
{
    Transaction transaction(
        5004,
        "Invalid transaction"
    );

    EXPECT_THROW(
        transaction.addEntry(
            1001,
            Money(0),
            Money(0)
        ),
        std::invalid_argument
    );
}

TEST(TransactionTest, StoresEntries)
{
    Transaction transaction(
        5005,
        "Entry test"
    );

    transaction.addEntry(
        1001,
        Money(10000),
        Money(0)
    );

    transaction.addEntry(
        1002,
        Money(0),
        Money(10000)
    );

    EXPECT_EQ(
        transaction.getEntries().size(),
        2
    );
}

TEST(TransactionTest, StoresIdAndDescription)
{
    Transaction transaction(
        5006,
        "Cash sale"
    );

    EXPECT_EQ(
        transaction.getId(),
        5006
    );

    EXPECT_EQ(
        transaction.getDescription(),
        "Cash sale"
    );
}