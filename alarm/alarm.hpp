#ifndef ALARM_HPP
#define ALARM_HPP

#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <chrono>

using namespace std;
using namespace std::chrono_literals;


typedef const chrono::duration<double, std::milli> TimePeriod;

enum Priority
{
    Low,
    Medium,
    High
};

void beepOrNoBeep(bool beep, TimePeriod duration);

class Alarm
{
private:
    TimePeriod beep_period;
    TimePeriod beep_duration;
    TimePeriod pause;
    int beep_repeat;

    // ATOMIC variable to have a well-defined behavior
    // between write and read by different threads
    atomic<bool> is_activated, is_started;
    atomic<int> beep_count;

    thread thr;
    Priority p;

    void beepTask();

public:
    Alarm(TimePeriod bp, TimePeriod bd, int br, TimePeriod _pause, Priority _p);

    void activate();
    void deactivate();
    void startAlarm();
    void stopAlarm();

    bool isActivated() const { return is_activated; }
    bool isStarted() const { return is_started; }
    
    Priority getPriority() const { return this->p;}
    std::thread::id getThreadId() const;  // used in unittests
    int getBeepCounter() const;           // used in unittests

    ~Alarm();
};

#endif