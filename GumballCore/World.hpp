#pragma once
#ifndef _world
#define _world

//#include "Object.hpp"
//#include "Definitions.hpp"
#include "Subsystem.hpp"
#include "Var.hpp"

#include <list>
using namespace std;

class World;
class Actor;
class Component;


class GBCORE GameObject {
public:
	virtual ~GameObject() {}
	virtual void beginPlay() {}
	virtual void endPlay() {}
	virtual void tick(const double &deltaTime) {}
};

class GBCORE Component : 
	public GameObject {
private:
	Actor *owner;
public:
	static void *operator new(unsigned long long sz);
	static void operator delete(void *ptr);
	Component();
	~Component();
};

class GBCORE Actor :
	public GameObject {
	friend class World;
private:
	list<Component*> components;
	Inline void componentTick(const double &deltaTime) {
		for (auto c : components)
			c->tick(deltaTime);
	}
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
