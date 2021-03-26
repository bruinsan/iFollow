// kbhit
#include <sys/ioctl.h> // For FIONREAD
#include <termios.h>
// end kbhit

#include <iostream>
#include <thread>
#include <chrono> // sleep
/******************** END IMPORTS **********************/
using namespace std;
using namespace std::chrono_literals;

class Timer
{
private:
    int period;
    int beep_duration;

public:
    Timer(/* args */);
    ~Timer();
};

Timer::Timer(/* args */)
{
}

Timer::~Timer()
{
}

void beep() { cout << "X"; }
void noBeep() { cout << "_"; }

void highPriority()
{
    // int i=0;
    while (1)
    {
        for (int i = 0; i < 5; i++)
        {
            beep();
            std::this_thread::sleep_for(500ms);
        }
        std::this_thread::sleep_for(2s);
    }
}

void mediumPriority()
{
    while (1)
    {
        beep();
        std::this_thread::sleep_for(1s);
    }
}

void lowPriority()
{
    while (1)
    {
        auto start_time = std::chrono::high_resolution_clock::now();
        auto after_time = std::chrono::high_resolution_clock::now();

        auto elapsed = after_time - start_time;

        cout << "here" << endl;
        while (elapsed < std::chrono::seconds(1))
        {
            beep();
            std::this_thread::sleep_for(250ms);
            after_time = std::chrono::high_resolution_clock::now();
            // std::chrono::duration<double, std::milli> elapsed = after_time - start_time;
            elapsed = after_time - start_time;

            // cout << after_time - start_time << endl;
        }
        std::this_thread::sleep_for(30s);
    }
}

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

    std::thread t1, t2, t3;
    while (1)
    {
        if (kbhit())
        {
            switch (c = getchar())
            {
            case 'h':
                cout << endl
                     << "High Priority TIMER" << endl;
                t1 = std::thread(&highPriority);
                break;
            case 'm':
                cout << endl
                     << "Medium Priority TIMER" << endl;
                t1 = std::thread(&mediumPriority);
                break;
            case 'l':
                cout << endl
                     << "Low Priority TIMER" << endl;
                t1 = std::thread(&lowPriority);
                break;
            case 'q':
                cout << endl
                     << "Stopping all Timers" << endl;
                // stop each timer and join threads
                break;
            default:
                break;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
        noBeep();
    }

    return 0;
}
