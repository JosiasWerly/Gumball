#pragma once
#ifndef _timestat
#include "GLUtils.hpp"
class TimeStat {
	double lastRegister = 0;
	
	double ms = 0;
	double deltaTime = 0;
	double fps = 0;
public:
	TimeStat() {
		lastRegister = glfwGetTime();
	}

	void capture() {
		const double actualTime = glfwGetTime();
		const double delta = actualTime - lastRegister;
		lastRegister = actualTime;


		ms = delta * 1000.0;
		fps = 1.0/delta;
		deltaTime = delta;
	}
	Inline const double &getDeltaTime() const {
		return deltaTime;
	}
	Inline const double &getMS() const {
		return ms;
	}
	Inline const double getFPS() const {
		return fps;
	}
};

#endif // !_timestat
