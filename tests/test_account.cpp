#include "models/Account.h"

#include <gtest/gtest.h>

TEST(AccountTest, AssetDebitIncreasesBalance)
{
    Account account(
        1001,
        "Cash",
        AccountType::ASSET
    );

    account.debit(Money(5000));

    EXPECT_EQ(
        account.getBalance().getPaise(),
        5000
    );
}

TEST(AccountTest, AssetCreditDecreasesBalance)
{
    Account account(
        1001,
        "Cash",
        AccountType::ASSET
    );

    account.debit(Money(5000));
    account.credit(Money(2000));

    EXPECT_EQ(
        account.getBalance().getPaise(),
        3000
    );
}

TEST(AccountTest, ExpenseDebitIncreasesBalance)
{
    Account account(
        1004,
        "Salary Expense",
        AccountType::EXPENSE
    );

    account.debit(Money(5000));

    EXPECT_EQ(
        account.getBalance().getPaise(),
        5000
    );
}

TEST(AccountTest, RevenueCreditIncreasesBalance)
{
    Account account(
        1003,
        "Sales",
        AccountType::REVENUE
    );

    account.credit(Money(5000));

    EXPECT_EQ(
        account.getBalance().getPaise(),
        5000
    );
}

TEST(AccountTest, RevenueDebitDecreasesBalance)
{
    Account account(
        1003,
        "Sales",
        AccountType::REVENUE
    );

    account.credit(Money(5000));
    account.debit(Money(2000));

    EXPECT_EQ(
        account.getBalance().getPaise(),
        3000
    );
}

TEST(AccountTest, LiabilityCreditIncreasesBalance)
{
    Account account(
        2001,
        "Loan",
        AccountType::LIABILITY
    );

    account.credit(Money(10000));

    EXPECT_EQ(
        account.getBalance().getPaise(),
        10000
    );
}

TEST(AccountTest, EquityCreditIncreasesBalance)
{
    Account account(
        3001,
        "Capital",
        AccountType::EQUITY
    );

    account.credit(Money(10000));

    EXPECT_EQ(
        account.getBalance().getPaise(),
        10000
    );
}