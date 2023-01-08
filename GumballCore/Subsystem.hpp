#pragma once
#ifndef _subsystem
#define _subsystem

class Engine;

//TODO: implement
enum class ESubsystemTick {
	disable,
	editor,
	gameplay,
	all
};
class GBCORE Subsystem {
protected:
	friend Engine;
	ESubsystemTick ticktype;

	Subsystem();
	virtual ~Subsystem();
	virtual void initialize();
	virtual void shutdown();
	virtual void beginPlay();
	virtual void endPlay();
	virtual void tick(const double &deltaTime);
};

#endif // !_subsystem
