#ifndef TIMER_HPP
#define TIMER_HPP

#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>

using namespace std;

enum Priority
{
    High,
    Medium,
    Low
};

class TimerManager
{
private:
public:
    TimerManager(){};
    ~TimerManager();
};

typedef const chrono::duration<double, std::milli> TimePeriod;

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

std::mutex mu;
void beepOrNoBeep(bool beep, TimePeriod duration)
{
    mu.lock();
    auto now = chrono::system_clock::now();
    auto max = now + duration;
    if (beep)
    {
        // auto elapsed_time = now + 0s;
        // int i=0;
        // cout << chrono::system_clock::to_time_t(max) << endl;
        while (now < max)
        {
            cout << "X";
            this_thread::sleep_for(250ms);
            now += 250ms;
        }
    }
    else
        cout << "_";
    mu.unlock();
}

void Timer::activate()
{
    this->is_activated = true;
    this->thr = thread(&Timer::startTimer, this);
}

void Timer::deactivate()
{
    this->is_activated = false;
    // we must test here because in case the timer is already deactivated, we don't have
    // any thread to join
    if (this->thr.joinable())
    {
        cout << "joining thread" << endl;
        this->thr.join();
    }
}

void Timer::startTimer()
{
    while (isActivated())
    {
        for (int i = 0; i < this->beep_repeat; i++)
        {
            // when the beep period is large, joining a thread will block until the end
            // of the period, we should use maybe condition_variable::wait_for()
            this_thread::sleep_for(this->beep_period);
            beepOrNoBeep(true, this->beep_duration);
        }
        // in the case pause != zero, we need to subtract the last beep_period from the pause
        // otherwise the interval will contain the pause + beep_period
        // (e.g. 2s + 500ms for the HIGH timer)
        if (this->pause != 0s)
            this_thread::sleep_for(this->pause - this->beep_period);
    }
}

#endif