#pragma once
#ifndef __timer
#include <chrono>

class GENGINE Timer {
    std::chrono::time_point<std::chrono::high_resolution_clock> time;

public:
    Timer();
    void start();
    float seconds() const;
    double milliseconds() const;
};

#endif // !__timer