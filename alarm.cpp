#include "alarm.hpp"

const char *priority_names[] = {"HIGH", "MEDIUM", "LOW"};

std::mutex mu; // mutex used on the shared function BeepOrNoBeep()
void beepOrNoBeep(bool beep, TimePeriod duration)
{
    { // scope to lock the mutex with unique_lock
        auto lock = std::unique_lock<std::mutex>(mu);
        auto start = chrono::system_clock::now();
        if (beep)
        {
            while (chrono::system_clock::now() - start < duration)
            {
                cout << "X";
                this_thread::sleep_for(250ms);
            }
        }
        else
            cout << "_";
    }
}

Alarm::Alarm(TimePeriod bp, TimePeriod bd, int br, TimePeriod _pause, Priority _p)
    : beep_period(bp), beep_duration(bd), beep_repeat(br), pause(_pause), p(_p)
{
    is_activated = false;
    beep_count = 0;
}

std::thread::id Alarm::getThreadId()
{
    return this->thr.get_id();
}

int Alarm::getBeepCounter()
{
    return beep_count;
}

void Alarm::activate()
{
    this->is_activated = true;
    this->thr = thread(&Alarm::startTimer, this);
}

typedef std::unique_lock<std::mutex> lock_type;
std::mutex mtx_ready;
std::condition_variable cv;
void Alarm::deactivate()
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

void Alarm::startTimer()
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
            {
                beep_count++;
                beepOrNoBeep(true, this->beep_duration);
            }
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

Alarm::~Alarm()
{
    cout << endl
         << "Destroying the alarm {" << priority_names[this->p] << "}" << endl;
    this->deactivate();
};
