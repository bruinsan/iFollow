#include "alarmManager.hpp"

const char *priority_names2[] = {"LOW", "MEDIUM", "HIGH"};

void AlarmManager::addAlarm(Alarm *a)
{
    this->alarms.push_back(a);
}

void AlarmManager::triggerAlarm(Alarm *a)
{
    bool higher_priority_started = false;
    // check if higher priorities are activated
    for (auto &i : alarms)
    {
        if (i->getPriority() > a->getPriority() && i->isStarted())
            higher_priority_started = true;
    }
    if (!a->isStarted() and !higher_priority_started)
    {
        cout << "\t" << priority_names2[a->getPriority()] << "\t";
        for (auto &al : alarms)
        {
            if (al->getPriority() < a->getPriority())
            {
                al->stopTimer();
            }
        }

        if (a->isActivated())
            // case where the alarm had already been activated and is only stopped
            a->startTimer();
        else
        {
            // first time we are starting the alarm
            a->activate();
            a->startTimer();
        }
    }
    else
    {
        // we're hitting the keyboard again to stop the alarm
        a->stopTimer();
        for (auto &al : alarms)
        {
            if (al->getPriority() < a->getPriority() && al->isActivated())
            {
                cout << "\t" << priority_names2[al->getPriority()] << "\t";

                al->startTimer();
                break; // we only want the next activated timer to be started
            }
        }
    }
}

void AlarmManager::deactivateAll()
{
    // only called when `s` are typed, stopping and deactivating all timers
    for (auto &i : alarms)
        i->deactivate();
}
