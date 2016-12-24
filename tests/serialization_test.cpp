#include "gtest/gtest.h"
#include "serialization.h"

TEST(test, sample_fails)
{
    ASSERT_EQ(1, 2);
}

TEST(test, sample_passes)
{
    ASSERT_EQ(1, 1);
}
