// kbhit
#include <sys/ioctl.h> // For FIONREAD
#include <termios.h>
// end kbhit

#include <iostream>
#include <thread>
#include <chrono> // sleep

#include "timer.hpp"
/******************** END IMPORTS **********************/
using namespace std;
using namespace std::chrono_literals;

void noBeep() { cout << "_"; }

/*
void highPriority()
{
    // int i=0;
    while (1)
    {
        for (int i = 0; i < 5; i++)
        {
            beep();
            this_thread::sleep_for(500ms);
        }
        this_thread::sleep_for(2s);
    }
}

void mediumPriority()
{
    while (1)
    {
        beep();
        this_thread::sleep_for(1s);
    }
}

void lowPriority()
{
    while (1)
    {
        auto start_time = chrono::high_resolution_clock::now();
        auto after_time = chrono::high_resolution_clock::now();

        auto elapsed = after_time - start_time;

        cout << "here" << endl;
        while (elapsed < chrono::seconds(1))
        {
            beep();
            this_thread::sleep_for(250ms);
            after_time = chrono::high_resolution_clock::now();
            // std::chrono::duration<double, std::milli> elapsed = after_time - start_time;
            elapsed = after_time - start_time;

            // cout << after_time - start_time << endl;
        }
        this_thread::sleep_for(30s);
    }
}
*/

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
    // setbuf(stdin, NULL);  // Optional: No buffering.

    //TODO: implement heap or linked list to the priority queue
    Timer tHigh(500ms, 250ms, 5, 2s, High);
    Timer tMedium(1s, 250ms, 1, 0s, Medium);
    Timer tLow(30s, 1s, 1, 0s, Low);

    while (true)
    {
        if (kbhit())
        {
            switch (c = getchar())
            {
            case 'h':
                if (!tHigh.isActivated())
                {
                    cout << endl
                         << "High Priority TIMER" << endl;
                    // must stop all others timers
                    tMedium.deactivate(); tLow.deactivate();
                    tHigh.activate();
                }
                else
                    tHigh.deactivate();
                break;

            case 'm':
                if (!tMedium.isActivated() && !tHigh.isActivated())
                {
                    cout << endl
                         << "Medium Priority TIMER" << endl;
                    // must stop low timer
                    tLow.deactivate();
                    tMedium.activate();
                }
                else
                    tMedium.deactivate();
                break;

            case 'l':
                if (!tLow.isActivated() && !tMedium.isActivated() && !tHigh.isActivated())
                {
                    cout << endl
                         << "Low Priority TIMER" << endl;
                    tLow.activate();
                }
                else
                    tLow.deactivate();
                break;

            case 'q':
                cout << endl
                     << "Stopping all Timers" << endl;
                tHigh.deactivate(); tMedium.deactivate(); tLow.deactivate();
                break;
            default:
                break;
            }
        }
        // cout << chrono::system_clock::to_time_t(chrono::system_clock::now()) << endl;
        beepOrNoBeep(false);
        this_thread::sleep_for(250ms);
    }

    return 0;
}
