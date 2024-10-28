#pragma once
#ifndef __timer
#include <chrono>

class GENGINE Timer {
    std::chrono::time_point<std::chrono::high_resolution_clock> time;
    float seconds;
    double milliseconds;

public:
    Timer();
    void start();
    float getSeconds() const { return seconds; }
    double getMilliseconds() const { return milliseconds; }
};

#endif // !__timer