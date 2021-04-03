#include "alarm.hpp"

const char *priority_names[] = {"LOW", "MEDIUM", "HIGH"};

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
    is_started = false;
    beep_count = 0;
}

std::thread::id Alarm::getThreadId() const
{
    return this->thr.get_id();
}

int Alarm::getBeepCounter() const
{
    return beep_count;
}

void Alarm::activate()
{
    this->is_activated = true;
    this->thr = thread(&Alarm::beepTask, this);
}

typedef std::unique_lock<std::mutex> lock_type;
std::mutex mtx_ready;
std::mutex mtx_stop;
std::condition_variable cv;

void Alarm::deactivate()
{
    this->is_activated = false;
    this->is_started = false;

    // we must test if joinable here because in case the timer is already deactivated,
    // we don't have any thread to join and an exception will be raised
    if (this->thr.joinable())
    {
        cv.notify_all();
        this->thr.join();
    }
}

void Alarm::startAlarm()
{
    if (!this->is_started)
        this->is_started = true;
}

void Alarm::stopAlarm()
{
    if (this->is_started)
    {
        this->is_started = false;
        cv.notify_all();
    }
}

void Alarm::beepTask()
{
    while (isActivated())
    {
        if (isStarted())
        {
            for (int i = 0; i < this->beep_repeat; i++)
            {
                lock_type lck(mtx_ready);
                // when the beep period is large, joining a thread will block until the end
                // of the period, we should use maybe condition_variable::wait_for()
                cv.wait_for(lck, this->beep_period);
                if (isActivated() && isStarted())
                {
                    beep_count++; // for testing purpose
                    beepOrNoBeep(true, this->beep_duration);
                }
                else
                {
                    // if the alarm becomes deactivated in the middle of a wait_for, we
                    // force a return(break) to immediately stop it
                    break;
                }
            }
            // In the case pause != zero, we need to subtract the last beep_period from the pause
            // otherwise the interval will contain the pause + beep_period
            // (e.g. 2s + 500ms for the HIGH timer)
            if (this->pause != 0s)
                this_thread::sleep_for(this->pause - this->beep_period);
        }
    }
}

Alarm::~Alarm()
{
    cout << endl
         << "Destroying the alarm {" << priority_names[this->p] << "}" << endl;
    this->deactivate();
}
