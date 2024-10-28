#include "Timer.hpp"

using namespace std;
using namespace chrono;

Timer::Timer() {
    time = high_resolution_clock::now();
}
void Timer::start() {
    milliseconds = duration_cast<std::chrono::milliseconds>(high_resolution_clock::now() - time).count();
    seconds = milliseconds / 1000.0;
    time = high_resolution_clock::now();
}