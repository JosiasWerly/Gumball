#pragma once
#ifndef _enginesystem
#define _enginesystem

class Engine;
class GBCORE IEngineSystem {
protected:
	friend Engine;

	IEngineSystem();
	virtual ~IEngineSystem();
	virtual void initialize();
	virtual void shutdown();
	virtual void onPlay();
	virtual void onEndplay();
	virtual void tick(const double &deltaTime);
};

#endif // !_enginesystem
