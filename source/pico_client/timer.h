#ifndef _TIMER_H_
#define _TIMER_H_

#include <chrono>

class _cTimer_ {
    std::chrono::system_clock::time_point tp0, tp1;
    std::chrono::duration<long double> elapsedTime;
public:
    _cTimer_(void) :
        elapsedTime(0.0)
    {
        /*Code...*/
    }
    ~_cTimer_(void) {
        /*Code...*/
    }

    void _begin_(void) {
        tp0 = std::chrono::system_clock::now();
    }

    void _end_(void) {
        tp1 = std::chrono::system_clock::now();
        elapsedTime = tp1 - tp0;
    }

    long double _elapsed_time_(void) {
        return(elapsedTime.count());
    }
};

#endif // _TIMER_H_
