#include <gtest/gtest.h>
#include "eyeblinking/utils/FixedSizeQueue.hpp"

TEST(FixedSizeQueueTest, TestPush)
{
    FixedSizeQueue<int, 3> q;
    q.push(1);
    q.push(2);
    q.push(3);
    q.push(4);
    ASSERT_EQ(q.size(), 3);
    ASSERT_EQ(q.front(), 2);
}

TEST(FixedSizeQueueTest, TestMean)
{
    FixedSizeQueue<int, 3> q;
    q.push(1);
    q.push(2);
    q.push(3);
    ASSERT_EQ(q.mean(), 2);
}

TEST(FixedSizeQueueTest, TestVariability)
{
    FixedSizeQueue<int, 3> q;
    q.push(1);
    q.push(2);
    q.push(3);
    ASSERT_EQ(q.variability(), 1);
}

TEST(FixedSizeQueueTest, TestClear)
{
    FixedSizeQueue<int, 3> q;
    q.push(1);
    q.push(2);
    q.push(3);
    q.clear();
    ASSERT_EQ(q.size(), 0);
}
