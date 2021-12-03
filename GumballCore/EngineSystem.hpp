#pragma once
#ifndef __enginesystem
#define __enginesystem

class IEngineSystem {
public:
	IEngineSystem() {}
	virtual ~IEngineSystem() {}

	virtual void Initialize() {}
	virtual void Shutdown() {}

	virtual void OnPlay() {}
	virtual void OnEndplay() {}
};

#endif // !__enginesystem
