#ifndef TIMER_H
#define TIMER_H

#ifdef  _WIN32
#include <windows.h>
#endif

class Timer{
public:
    Timer();
    void start();
    void stop();
    double elapsed() const;
    void reset();
private:
    bool started;
#ifdef _WIN32
    LARGE_INTEGER m_start;
    LARGE_INTEGER m_stop;
#else
    double m_start;
    double m_stop;
#endif
};

#endif
