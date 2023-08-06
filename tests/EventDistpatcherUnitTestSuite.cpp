#include "eyeblinking/events/Dispatcher.hpp"
#include "gtest/gtest.h"

TEST(DispatcherTest, TestSubscribe)
{
    Dispatcher<int> dispatcher;
    auto handler = []([[maybe_unused]] const Event<int>& e) { /* handle the event */ };
    dispatcher.subscribe(handler);
    ASSERT_EQ(1, dispatcher.getObservers().size());
}

TEST(DispatcherTest, TestPost)
{
    Dispatcher<int> dispatcher;
    int count = 0;
    auto handler1 = [&count]([[maybe_unused]] const Event<int>& e) { count++; };
    auto handler2 = [&count]([[maybe_unused]] const Event<int>& e) { count++; };
    dispatcher.subscribe(handler1);
    dispatcher.subscribe(handler2);

    Event<int> event(42);
    dispatcher.post(event);
    ASSERT_EQ(2, count);
}

TEST(DispatcherTest, TestDisconnect)
{
    Dispatcher<int> dispatcher;
    auto handler = []([[maybe_unused]] const Event<int>& e) { /* handle the event */ };
    dispatcher.subscribe(handler);
    ASSERT_EQ(1, dispatcher.getObservers().size());
    dispatcher.disconnect();
    ASSERT_EQ(0, dispatcher.getObservers().size());
}
