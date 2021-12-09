#pragma once
#ifndef __enginesystem
#define __enginesystem

class IEngineSystem {
public:
	IEngineSystem() {}
	virtual ~IEngineSystem() {}

	virtual void initialize() {}
	virtual void shutdown() {}

	virtual void onPlay() {}
	virtual void onEndplay() {}
};

#endif // !__enginesystem
