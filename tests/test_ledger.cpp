#include "core/Ledger.h"

#include <filesystem>
#include <gtest/gtest.h>
#include <string>
#include <thread>
#include <vector>

TEST(LedgerTest, PostsValidTransaction)
{
    Ledger ledger;

    Account cash(
        1001,
        "Cash",
        AccountType::ASSET
    );

    Account sales(
        1003,
        "Sales",
        AccountType::REVENUE
    );

    ledger.addAccount(cash);
    ledger.addAccount(sales);

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

    EXPECT_TRUE(
        ledger.postTransaction(transaction)
    );

    EXPECT_EQ(
        ledger.getAccount(1001)->getBalance().getPaise(),
        10000
    );

    EXPECT_EQ(
        ledger.getAccount(1003)->getBalance().getPaise(),
        10000
    );
}

TEST(LedgerTest, RejectsUnbalancedTransaction)
{
    Ledger ledger;

    ledger.addAccount(
        Account(
            1001,
            "Cash",
            AccountType::ASSET
        )
    );

    ledger.addAccount(
        Account(
            1003,
            "Sales",
            AccountType::REVENUE
        )
    );

    Transaction transaction(
        5002,
        "Invalid sale"
    );

    transaction.addEntry(
        1001,
        Money(10000),
        Money(0)
    );

    transaction.addEntry(
        1003,
        Money(0),
        Money(5000)
    );

    EXPECT_FALSE(
        ledger.postTransaction(transaction)
    );

    EXPECT_EQ(
        ledger.getAccount(1001)->getBalance().getPaise(),
        0
    );

    EXPECT_EQ(
        ledger.getAccount(1003)->getBalance().getPaise(),
        0
    );
}

TEST(LedgerTest, RejectsUnknownAccount)
{
    Ledger ledger;

    ledger.addAccount(
        Account(
            1001,
            "Cash",
            AccountType::ASSET
        )
    );

    Transaction transaction(
        5003,
        "Invalid account"
    );

    transaction.addEntry(
        1001,
        Money(10000),
        Money(0)
    );

    transaction.addEntry(
        9999,
        Money(0),
        Money(10000)
    );

    EXPECT_FALSE(
        ledger.postTransaction(transaction)
    );

    EXPECT_EQ(
        ledger.getAccount(1001)->getBalance().getPaise(),
        0
    );
}

TEST(LedgerTest, FailedTransactionDoesNotPartiallyModifyAccounts)
{
    Ledger ledger;

    ledger.addAccount(
        Account(
            1001,
            "Cash",
            AccountType::ASSET
        )
    );

    Transaction transaction(
        5004,
        "Invalid transaction"
    );

    transaction.addEntry(
        1001,
        Money(10000),
        Money(0)
    );

    transaction.addEntry(
        9999,
        Money(0),
        Money(10000)
    );

    EXPECT_FALSE(
        ledger.postTransaction(transaction)
    );

    EXPECT_EQ(
        ledger.getAccount(1001)->getBalance().getPaise(),
        0
    );
}

TEST(LedgerTest, StoresPostedTransaction)
{
    Ledger ledger;

    ledger.addAccount(
        Account(
            1001,
            "Cash",
            AccountType::ASSET
        )
    );

    ledger.addAccount(
        Account(
            1002,
            "Capital",
            AccountType::EQUITY
        )
    );

    Transaction transaction(
        5005,
        "Initial capital"
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
        ledger.postTransaction(transaction)
    );

    EXPECT_EQ(
        ledger.getTransactions().size(),
        1
    );

    EXPECT_EQ(
        ledger.getTransactions()[0].getId(),
        5005
    );
}


class LedgerRecoveryTest : public ::testing::Test
{
protected:
    std::string walPath;

    void SetUp() override
    {
        const auto *testInfo =
            ::testing::UnitTest::GetInstance()->current_test_info();

        walPath =
            "data/test_recovery_" +
            std::string(testInfo->name()) +
            ".log";

        std::filesystem::remove(walPath);
    }

    void TearDown() override
    {
        std::filesystem::remove(walPath);
    }
};

TEST_F(LedgerRecoveryTest, RecoversTransactionFromWAL)
{
    WAL wal(walPath);

    Ledger ledger(nullptr, &wal);

    ledger.addAccount(
        Account(
            1001,
            "Cash",
            AccountType::ASSET
        )
    );

    ledger.addAccount(
        Account(
            1003,
            "Revenue",
            AccountType::REVENUE
        )
    );

    Transaction transaction(
        5001,
        "Recovered sale"
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

    ledger.recover();

    Account *cash =
        ledger.getAccount(1001);

    Account *revenue =
        ledger.getAccount(1003);

    ASSERT_NE(cash, nullptr);
    ASSERT_NE(revenue, nullptr);

    EXPECT_EQ(
        cash->getBalance().getPaise(),
        10000
    );

    EXPECT_EQ(
        revenue->getBalance().getPaise(),
        10000
    );

    EXPECT_EQ(
        ledger.getTransactions().size(),
        1
    );
}

TEST_F(LedgerRecoveryTest, DoesNotRecoverAlreadyAppliedTransaction)
{
    WAL wal(walPath);

    Ledger ledger(nullptr, &wal);

    ledger.addAccount(
        Account(
            1001,
            "Cash",
            AccountType::ASSET
        )
    );

    ledger.addAccount(
        Account(
            1003,
            "Revenue",
            AccountType::REVENUE
        )
    );

    Transaction transaction(
        5001,
        "Sale"
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

    ledger.postTransaction(transaction);

    ledger.recover();

    Account *cash =
        ledger.getAccount(1001);

    ASSERT_NE(cash, nullptr);

    EXPECT_EQ(
        cash->getBalance().getPaise(),
        10000
    );

    EXPECT_EQ(
        ledger.getTransactions().size(),
        1
    );
}

TEST_F(LedgerRecoveryTest, RejectsRecoveryWithUnknownAccount)
{
    WAL wal(walPath);

    Ledger ledger(nullptr, &wal);

    ledger.addAccount(
        Account(
            1001,
            "Cash",
            AccountType::ASSET
        )
    );

    Transaction transaction(
        5001,
        "Invalid recovery"
    );

    transaction.addEntry(
        1001,
        Money(10000),
        Money(0)
    );

    transaction.addEntry(
        9999,
        Money(0),
        Money(10000)
    );

    wal.append(transaction);

    ledger.recover();

    Account *cash =
        ledger.getAccount(1001);

    ASSERT_NE(cash, nullptr);

    EXPECT_EQ(
        cash->getBalance().getPaise(),
        0
    );

    EXPECT_EQ(
        ledger.getTransactions().size(),
        0
    );
}

TEST(LedgerTest, HandlesConcurrentTransactions)
{
    Ledger ledger;

    ledger.addAccount(
        Account(
            1001,
            "Cash",
            AccountType::ASSET
        )
    );

    ledger.addAccount(
        Account(
            1002,
            "Capital",
            AccountType::EQUITY
        )
    );

    const int threadCount = 10;
    const int transactionsPerThread = 100;

    std::vector<std::thread> threads;

    for (int threadId = 0;
         threadId < threadCount;
         ++threadId)
    {
        threads.emplace_back(
            [&ledger, threadId]()
            {
                for (int i = 0;
                     i < transactionsPerThread;
                     ++i)
                {
                    int transactionId =
                        threadId * transactionsPerThread + i + 1;

                    Transaction transaction(
                        transactionId,
                        "Concurrent transaction"
                    );

                    transaction.addEntry(
                        1001,
                        Money(100),
                        Money(0)
                    );

                    transaction.addEntry(
                        1002,
                        Money(0),
                        Money(100)
                    );

                    EXPECT_TRUE(
                        ledger.postTransaction(transaction)
                    );
                }
            }
        );
    }

    for (auto &thread : threads)
    {
        thread.join();
    }

    EXPECT_EQ(
        ledger.getTransactions().size(),
        1000
    );

    EXPECT_EQ(
        ledger.getAccount(1001)->getBalance().getPaise(),
        100000
    );

    EXPECT_EQ(
        ledger.getAccount(1002)->getBalance().getPaise(),
        100000
    );
}

TEST(LedgerTest, HandlesConcurrentReadsAndWrites)
{
    Ledger ledger;

    ledger.addAccount(
        Account(
            1001,
            "Cash",
            AccountType::ASSET
        )
    );

    ledger.addAccount(
        Account(
            1002,
            "Capital",
            AccountType::EQUITY
        )
    );

    const int writerThreadCount = 5;
    const int readerThreadCount = 5;
    const int transactionsPerWriter = 100;

    std::vector<std::thread> threads;

    for (int threadId = 0;
         threadId < writerThreadCount;
         ++threadId)
    {
        threads.emplace_back(
            [&ledger, threadId]()
            {
                for (int i = 0;
                     i < transactionsPerWriter;
                     ++i)
                {
                    int transactionId =
                        threadId * transactionsPerWriter + i + 1;

                    Transaction transaction(
                        transactionId,
                        "Concurrent transaction"
                    );

                    transaction.addEntry(
                        1001,
                        Money(100),
                        Money(0)
                    );

                    transaction.addEntry(
                        1002,
                        Money(0),
                        Money(100)
                    );

                    ledger.postTransaction(transaction);
                }
            }
        );
    }

    for (int i = 0;
         i < readerThreadCount;
         ++i)
    {
        threads.emplace_back(
            [&ledger]()
            {
                for (int j = 0;
                     j < 100;
                     ++j)
                {
                    ledger.getAccounts();
                    ledger.getTransactions();
                    ledger.hasAccount(1001);
                    ledger.getAccount(1001);
                }
            }
        );
    }

    for (auto &thread : threads)
    {
        thread.join();
    }

    EXPECT_EQ(
        ledger.getTransactions().size(),
        500
    );

    EXPECT_EQ(
        ledger.getAccount(1001)->getBalance().getPaise(),
        50000
    );

    EXPECT_EQ(
        ledger.getAccount(1002)->getBalance().getPaise(),
        50000
    );
}