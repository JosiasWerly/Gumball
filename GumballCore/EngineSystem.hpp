#pragma once
#ifndef _enginesystem
#define _enginesystem

class Engine;
class IEngineSystem {
protected:
	friend Engine;

	IEngineSystem() { }
	virtual ~IEngineSystem() { }

	virtual void initialize() {}
	virtual void shutdown() {}
	virtual void tick(float deltaTime) {}

	virtual void onPlay() {}
	virtual void onEndplay() {}
};

#endif // !_enginesystem
