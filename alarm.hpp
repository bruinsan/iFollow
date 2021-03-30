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
    High,
    Medium,
    Low
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
    atomic<bool> is_activated;
    atomic<int> beep_count;

    Priority p;
    thread thr;

public:
    Alarm(TimePeriod bp, TimePeriod bd, int br, TimePeriod _pause, Priority _p);

    void activate();
    void deactivate();

    bool isActivated() { return is_activated; }
    std::thread::id getThreadId();
    int getBeepCounter();

    void startTimer();
    void stopTimer();
    ~Alarm();
};

#endif