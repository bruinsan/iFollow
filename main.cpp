// kbhit
#include <sys/ioctl.h> // For FIONREAD
#include <termios.h>
// end kbhit

#include <iostream>
#include <chrono> // sleep

#include "alarm.hpp"
/******************** END IMPORTS **********************/
using namespace std;
using namespace std::chrono_literals;


/*
* function from : https://stackoverflow.com/questions/421860/
* capture-characters-from-standard-input-without-waiting-for-enter-to-be-pressed/33201364#33201364
* inspired on conio.h library for windows
*/
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

    //TODO: implement heap or linked list to the priority queue
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
                if (!tHigh.isActivated())
                {
                    cout << endl
                         << "\t\tHIGH\t";
                    // must stop all others timers
                    tMedium.deactivate();
                    tLow.deactivate();
                    tHigh.activate();
                }
                else
                    tHigh.deactivate();
                break;

            case 'm':
                if (!tMedium.isActivated() && !tHigh.isActivated())
                {
                    cout << endl
                         << "\t\tMEDIUM\t";

                    tLow.deactivate(); // must stop low timer
                    tMedium.activate();
                }
                else
                    tMedium.deactivate();
                break;

            case 'l':
                if (!tLow.isActivated() && !tMedium.isActivated() && !tHigh.isActivated())
                {
                    cout << endl
                         << "\t\tLOW\t";
                    tLow.activate();
                }
                else
                    tLow.deactivate();
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
