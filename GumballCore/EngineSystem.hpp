#pragma once
#ifndef __enginesystem
#define __enginesystem


class Engine;
class IEngineSystem {
protected:
	friend Engine;

	IEngineSystem() { }
	virtual ~IEngineSystem() { }

	virtual void initialize() {}
	virtual void shutdown() {}
	virtual void tick() {}

	virtual void onPlay() {}
	virtual void onEndplay() {}

};

#endif // !__enginesystem
