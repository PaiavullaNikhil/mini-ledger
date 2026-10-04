#include "models/Money.h"

#include <gtest/gtest.h>

TEST(MoneyTest, StoresPaise)
{
    Money money(5000);

    EXPECT_EQ(
        money.getPaise(),
        5000
    );
}

TEST(MoneyTest, Addition)
{
    Money a(5000);
    Money b(3000);

    Money result = a + b;

    EXPECT_EQ(
        result.getPaise(),
        8000
    );
}

TEST(MoneyTest, Zero)
{
    Money money(0);

    EXPECT_TRUE(
        money.isZero()
    );
}