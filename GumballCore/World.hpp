#pragma once
#ifndef _world
#define _world

//#include "Object.hpp"
#include "Definitions.hpp"
#include "Subsystem.hpp"
#include "Var.hpp"

#include <list>
using namespace std;

class World;
class Level;
class IComponent;
class Actor;


class GBCORE GameObject {
public:
	virtual ~GameObject() {}
	virtual void beginPlay() {}
	virtual void endPlay() {}
	virtual void tick(const double &deltaTime) {}
};

class GBCORE Actor :
	public GameObject {
public:
	static void *operator new(unsigned long long sz);
	static void operator delete(void *ptr);
	Actor();
	virtual ~Actor();
};

class World : 
	public Subsystem {
	friend class Actor;

	list<Var<Actor>> actors;
	list<Var<Actor>*> toSpawn, toDelete, toTick;

	void registerActor(Actor *actor);
	void unregisterActor(Actor *actor);
public:
	virtual void initialize() override;
	virtual void shutdown() override;
	virtual void beginPlay() override;
	virtual void endPlay() override;
	virtual void tick(const double &deltaTime) override;
};
#endif // _world
