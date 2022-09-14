#include "TimeStat.hpp"

TimeStat::TimeStat() {
	lastRegister = glfwGetTime();
}

void TimeStat::capture() {
	const double actualTime = glfwGetTime();
	const double delta = actualTime - lastRegister;
	lastRegister = actualTime;


	ms = delta * 1000.0;
	fps = 1.0 / delta;
	deltaTime = delta;
}
const double &TimeStat::getDeltaTime() const {
	return deltaTime;
}
const double &TimeStat::getMS() const {
	return ms;
}
const double TimeStat::getFPS() const {
	return fps;
}