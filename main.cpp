// kbhit
#include <sys/ioctl.h> // For FIONREAD
#include <termios.h>
// end kbhit

#include <iostream>

#include "alarm.hpp"

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
    while (c != 'Q')
    {
        if (kbhit())
        {
            switch (c = getchar())
            {
            case 'h':
                if (!tHigh.isStarted())
                {
                    cout << endl
                         << "\t\tHIGH\t";
                    // must stop all others timers
                    tMedium.stopTimer(); // maybe stop just
                    tLow.stopTimer();
                    if (tHigh.isActivated())
                    {
                        tHigh.startTimer();
                    }
                    else
                    {
                        tHigh.activate();
                        tHigh.startTimer();
                    }
                }
                else
                {
                    tHigh.stopTimer();
                    if (tMedium.isActivated())
                        tMedium.startTimer();
                    if (!tMedium.isActivated() && tLow.isActivated())
                        tLow.startTimer();
                }
                // alarm_manager.triggerAlarm(High)
                break;

            case 'm':
                if (!tMedium.isStarted() && !tHigh.isStarted())
                {
                    cout << endl
                         << "\t\tMEDIUM\t";

                    tLow.stopTimer(); // must stop low timer
                    if (tMedium.isActivated())
                    {
                        tMedium.startTimer();
                    }
                    else
                    {
                        tMedium.activate();
                        tMedium.startTimer();
                    }
                }
                else
                {
                    tMedium.stopTimer();
                    if (tLow.isActivated())
                        tLow.startTimer();
                }
                break;

            case 'l':
                if (!tLow.isStarted() && !tMedium.isStarted() && !tHigh.isStarted())
                {
                    cout << endl
                         << "\t\tLOW\t";
                    if (tLow.isActivated())
                        tLow.startTimer();
                    else
                    {
                        tLow.activate();
                        tLow.startTimer();
                    }
                }
                else
                    tLow.stopTimer();
                break;

            case 's':
                cout << endl
                     << "Stopping all Timers\t";
                tHigh.deactivate();
                tMedium.deactivate();
                tLow.deactivate();
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
