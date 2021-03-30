#include "alarm.hpp"
#include <gtest/gtest.h>

TEST(ThreadTest, DifferentThreads)
{
    Alarm a1(500ms, 250ms, 5, 2s, High);
    a1.activate();

    ASSERT_NE(a1.getThreadId(), std::this_thread::get_id());

}

TEST(BeepTest, CountBeep)
{
    auto sleep_time = 10s;
    Alarm a1(250ms, 0ms, 1, 0s, Medium);
    a1.activate();
    auto start_point = chrono::system_clock::now();
    std::this_thread::sleep_for(sleep_time);
    
    ASSERT_EQ(a1.getBeepCounter(), sleep_time/250ms-1);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}