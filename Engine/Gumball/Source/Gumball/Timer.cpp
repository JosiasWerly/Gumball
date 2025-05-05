#include "Timer.hpp"

using namespace std;
using namespace chrono;

Timer::Timer() {
}
void Timer::start() {
    time = high_resolution_clock::now();
}
float Timer::seconds() const {
    return duration_cast<std::chrono::seconds>(high_resolution_clock::now() - time).count(); 
}
double Timer::milliseconds() const { 
    return duration_cast<std::chrono::milliseconds>(high_resolution_clock::now() - time).count(); 
}