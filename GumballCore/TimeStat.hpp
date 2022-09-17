#pragma once
#ifndef _timestat
#include "GLUtils.hpp"
class GBCORE TimeStat {
	double lastRegister = 0;
	
	double ms = 0;
	double deltaTime = 0;
	double fps = 0;
public:
	TimeStat();
	void capture();
	const double &getDeltaTime() const;
	const double &getMS() const;
	const double getFPS() const;
};

#endif // !_timestat
