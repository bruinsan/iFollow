// kbhit
#include <sys/ioctl.h> // For FIONREAD
#include <termios.h>
// end kbhit

#include <iostream>

#include "alarm/alarm.hpp"
#include "alarm/alarmManager.hpp"

// function from : https://bit.ly/3maDF2A inspired on conio.h library for windows
int kbhit(void)
{
    static bool initflag = false;
    static const int STDIN = 0;

    if (!initflag)
    {
        // Use termios to turn off line buffering
        struct termios term;
        tcgetattr(STDIN, &term);
        term.c_lflag &= ~ICANON;
        tcsetattr(STDIN, TCSANOW, &term);
        setbuf(stdin, NULL);
        initflag = true;
    }

    int nbbytes;
    ioctl(STDIN, FIONREAD, &nbbytes); // 0 is STDIN
    return nbbytes;
}

int main(int argc, char **argv)
{
    char c;
    setbuf(stdout, NULL); // No buffering.

    Alarm tHigh(500ms, 250ms, 5, 2s, High);
    Alarm tMedium(1s, 250ms, 1, 0s, Medium);
    Alarm tLow(30s, 1s, 1, 0s, Low);

    AlarmManager alarm_manager;

    alarm_manager.addAlarm(&tHigh);
    alarm_manager.addAlarm(&tMedium);
    alarm_manager.addAlarm(&tLow);

    while (c != 'Q')
    {
        if (kbhit())
        {
            switch (c = getchar())
            {
            case 'h':
                alarm_manager.triggerAlarm(&tHigh);
                break;

            case 'm':
                alarm_manager.triggerAlarm(&tMedium);

                break;

            case 'l':
                alarm_manager.triggerAlarm(&tLow);

                break;

            case 's':
                cout << endl
                     << "Stopping all Timers\t";
                alarm_manager.deactivateAll();
                break;

            default:
                break;
            }
        }
        beepOrNoBeep(false, 0s);
        this_thread::sleep_for(250ms);
    }

    return 0;
}
