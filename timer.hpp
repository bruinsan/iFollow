#ifndef TIMER_HPP
#define TIMER_HPP

#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>

using namespace std;

typedef const chrono::duration<double, std::milli> TimePeriod;

void beepOrNoBeep(bool beep, TimePeriod duration);

enum Priority
{
    High,
    Medium,
    Low
};

class Timer
{
private:
    TimePeriod beep_period;
    TimePeriod beep_duration;
    TimePeriod pause;

    int beep_repeat;

    // ATOMIC variable to give a well-defined behavior between write and read by different threads
    atomic<bool> is_activated;
    Priority p;

    thread thr; // TODO : use unique_ptr<thread>

public:
    Timer(TimePeriod bp, TimePeriod bd, int br, TimePeriod _pause, Priority _p)
        : beep_period(bp), beep_duration(bd), beep_repeat(br), pause(_pause), p(_p)
    {
        is_activated = false;
    }
    void activate();
    void deactivate();

    bool isActivated() { return is_activated; }

    void startTimer();
    void stopTimer();
    ~Timer(){};
};

void Timer::activate()
{
    this->is_activated = true;
    this->thr = thread(&Timer::startTimer, this);
}

typedef std::unique_lock<std::mutex> lock_type;
std::mutex mtx_ready;
std::mutex mtx_work;
std::condition_variable cv;
bool ready = false;

void Timer::deactivate()
{
    this->is_activated = false;

    // we must test fi joinable here because in case the timer is already deactivated,
    // we don't have any thread to join and an exception will be raised
    if (this->thr.joinable())
    {
        cv.notify_all();
        this->thr.join();
    }
}

void Timer::startTimer()
{
    while (isActivated())
    {
        for (int i = 0; i < this->beep_repeat; i++)
        {
            lock_type lck(mtx_ready);
            // when the beep period is large, joining a thread will block until the end
            // of the period, we should use maybe condition_variable::wait_for()
            cv.wait_for(lck, this->beep_period);
            if (isActivated())
                beepOrNoBeep(true, this->beep_duration);
            else
                return;
        }
        // in the case pause != zero, we need to subtract the last beep_period from the pause
        // otherwise the interval will contain the pause + beep_period
        // (e.g. 2s + 500ms for the HIGH timer)
        if (this->pause != 0s)
            this_thread::sleep_for(this->pause - this->beep_period);
    }
}

#endif