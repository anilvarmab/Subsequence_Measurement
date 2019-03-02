#include "timer.h"

#include <cassert>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

#ifdef _WIN32
Timer::Timer() : started(false) {
    m_start.QuadPart = 0;
    m_start.QuadPart = 0;
}
#else
Timer::Timer() : started(false), m_start(0.0), m_stop(0.0){}
#endif

void Timer::start() {
    assert(started==false);
    started = true;
  
#ifdef _WIN32
    QueryPerformanceCounter(&m_start);
#else
    timeval start;
    gettimeofday(&start, 0);
    m_start = 1.e6*static_cast<double>(start.tv_sec)
            + static_cast<double>(start.tv_usec);
#endif
}

void Timer::stop() {
    assert(started==true);
    started = false;
#ifdef _WIN32
    QueryPerformanceCounter(&m_stop);
#else
    timeval stop;
    gettimeofday(&stop, 0);
    m_stop = 1.e6*static_cast<double>(stop.tv_sec)
           + static_cast<double>(stop.tv_usec);
#endif
}

double Timer::elapsed() const {
    assert(started==false);
#ifdef _WIN32
    LARGE_INTEGER elapsedMicroseconds;
    elapsedMicroseconds.QuadPart = m_stop.QuadPart - m_start.QuadPart;
    elapsedMicroseconds.QuadPart *= 1000000;
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    elapsedMicroseconds.QuadPart /= frequency.QuadPart;
    return static_cast<double>(elapsedMicroseconds.QuadPart);
#else
    return (m_stop-m_start);
#endif
}

void Timer::reset() {
    assert(started==false);
#ifdef _WIN32
    m_start.QuadPart =0;
    m_stop.QuadPart = 0;
#else
    m_start=0.0;
    m_stop=0.0;
#endif
}
