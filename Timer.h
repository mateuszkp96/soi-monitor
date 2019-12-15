#ifndef MONITOR_TIMER_H
#define MONITOR_TIMER_H

#include <chrono>

class Timer {
public:
    static Timer& getInstance();

    int getTimeFromBegin() {
        std::chrono::time_point<std::chrono::steady_clock> now = std::chrono::steady_clock::now();
        std::chrono::microseconds time (std::chrono::duration_cast<std::chrono::microseconds>(now - begin_));
        return time.count();
    }
private:
    Timer() : begin_(std::chrono::steady_clock::now()) {}
    Timer(const Timer&) = delete;
    Timer& operator=(const Timer&) = delete;

    std::chrono::time_point<std::chrono::steady_clock> begin_;
};

Timer& Timer::getInstance() {
    static Timer instance;
    return instance;
}

#endif //MONITOR_TIMER_H
