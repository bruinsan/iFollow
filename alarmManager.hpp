#ifndef ALARM_MANAGER_HPP
#define ALARM_MANAGER_HPP

#include <vector>
#include "alarm.hpp"

class AlarmManager
{
private:
    vector<Alarm* > alarms;

public:
    AlarmManager() {}

    void addAlarm(Alarm* a);

    void triggerAlarm(Alarm* a);

    void deactivateAll();

    ~AlarmManager() {}
};

#endif