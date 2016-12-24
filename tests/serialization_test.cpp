#include "gtest/gtest.h"

TEST(test, sample_fails)
{
    ASSERT_EQ(1, 2);
}

TEST(test, sample_passes)
{
    ASSERT_EQ(1, 1);
}
