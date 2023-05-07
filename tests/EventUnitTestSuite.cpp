#include "eyeblinking/events/Event.hpp"
#include "gtest/gtest.h"

TEST(EventTest, TestType)
{
    Event event(42);
    EXPECT_EQ(42, event.type());
}

TEST(EventTest, TestName)
{
    Event event(42, "test");
    EXPECT_EQ("test", event.getName());
}
