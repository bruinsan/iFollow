#include "alarm.hpp"
#include "alarmManager.hpp"
#include <gtest/gtest.h>

TEST(AlarmManagerTest, DifferentThreads)
{
    /***
    Check each alarm creates a new thread

    Steps: 
        - Create 3 alarms (Alarm)
        - Add them to AlarmManager
        - Trigger them from Low to High
        - Get threadID

    Expected:
        - Each thread created has a different ID also
        different from the main thread

    **/
    Alarm a1(500ms, 250ms, 5, 2s, High);
    Alarm a2(500ms, 250ms, 5, 2s, Medium);
    Alarm a3(500ms, 250ms, 5, 2s, Low);

    AlarmManager am;

    am.addAlarm(&a1);
    am.addAlarm(&a2);
    am.addAlarm(&a3);

    am.triggerAlarm(&a3);
    am.triggerAlarm(&a2);
    am.triggerAlarm(&a1);

    ASSERT_NE(a1.getThreadId(), std::this_thread::get_id());
    ASSERT_NE(a1.getThreadId(), a2.getThreadId());
    ASSERT_NE(a1.getThreadId(), a3.getThreadId());
    ASSERT_NE(a2.getThreadId(), a3.getThreadId());
}

TEST(BeepTest, CountBeep)
{
    /***
    Ensure alarm are correctly beeping on time

    Steps:
        - Create an Alarm (beeping each 250ms)
        - Start it
        - After 10 seconds, count how many beeps it did

    Expected:
        - The number of beeps should be equal to (sleep_time/beep_time - 1)

    ***/
    auto sleep_time = 10s;
    auto beep_period = 250ms;

    Alarm a1(250ms, 0ms, 1, 0s, Medium);
    a1.activate();
    a1.startTimer();

    std::this_thread::sleep_for(sleep_time);

    ASSERT_EQ(a1.getBeepCounter(), sleep_time / beep_period - 1);
}

TEST(AlarmManagerTest, OnlyHigherPriorityAlarmPlayed)
{
    /***
    Check the priority between alarms is respected and only the higher
    activated alarm is played each time

    Steps:
        - Create 3 Alarms (Low, Medium and High priorities)
        - Start each alarm (low->high)
        - Check the alarm state (isStarted)

    Expected:
        - Each time when a higher priority alarm is started
        the lower is stopped
    ***/
    Alarm a1(500ms, 250ms, 5, 2s, High);
    Alarm a2(500ms, 250ms, 5, 2s, Medium);
    Alarm a3(500ms, 250ms, 5, 2s, Low);

    AlarmManager am;

    am.addAlarm(&a1);
    am.addAlarm(&a2);
    am.addAlarm(&a3);

    am.triggerAlarm(&a3);
    ASSERT_TRUE(a3.isStarted());

    am.triggerAlarm(&a2);
    ASSERT_TRUE(a2.isStarted());
    ASSERT_FALSE(a3.isStarted());

    am.triggerAlarm(&a1);
    ASSERT_TRUE(a1.isStarted());
    ASSERT_FALSE(a2.isStarted());
    ASSERT_FALSE(a3.isStarted());
}

TEST(AlarmManagerTest, RestartActivatedAlarm)
{
    /***
    Check when 2 or more alarms are activated, stopping the higher one makes
    the lower priority alarm starts
    

    Steps:
        - Create 3 Alarms (Low, Medium and High priorities)
        - Start each alarm (low->high)
        - Stop each alarm (high->low)
        - Check the alarm state (isStated)

    Expected:
        - Each time when a higher priority alarm is stopped and a lower alarm is
        activated, it should start
    ***/
    Alarm a1(500ms, 250ms, 5, 2s, High);
    Alarm a2(500ms, 250ms, 5, 2s, Medium);
    Alarm a3(500ms, 250ms, 5, 2s, Low);

    AlarmManager am;

    am.addAlarm(&a1);
    am.addAlarm(&a2);
    am.addAlarm(&a3);

    // starting alarms
    am.triggerAlarm(&a3);
    am.triggerAlarm(&a2);
    am.triggerAlarm(&a1);

    // stopping alarms
    am.triggerAlarm(&a1);
    ASSERT_FALSE(a1.isStarted());
    ASSERT_TRUE(a2.isStarted());

    am.triggerAlarm(&a2);
    ASSERT_FALSE(a2.isStarted());
    ASSERT_TRUE(a3.isStarted());

    am.triggerAlarm(&a3);
    ASSERT_FALSE(a3.isStarted());
}
int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}