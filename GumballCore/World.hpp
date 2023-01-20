#pragma once
#ifndef _world
#define _world

//#include "Object.hpp"
//#include "Definitions.hpp"
#include "Subsystem.hpp"
#include "Var.hpp"

#include <list>
using namespace std;

class GameObject;
class EntitySubsystem;
class World;

enum class EGameObjectState {
	spawn, nominal, destroy
};
class GBCORE GameObject {
	friend class EntitySubsystem;
	EGameObjectState state;
	string name;

public:
	GameObject();
	virtual ~GameObject();
	virtual void beginPlay();
	virtual void endPlay();
	virtual void tick(const double &deltaTime);	

	string getName();
	void setName(string name);
	const bool isValid() const { return state == EGameObjectState::nominal; }

	static void *operator new(unsigned long long sz);
};
Extern GBCORE void destroy(GameObject *go); //I would like to use the keyword delete, however my brain is moody today.

class GBCORE EntitySubsystem {
	friend class GameObject;
	list<Var<GameObject>> entities;
	list<Var<GameObject> *> toBegin, toEnd, toTick;

	string requestName(const string &suggestedName);
public:
	void unload();
	void add(GameObject *go);
	void del(GameObject *go);
	Inline void tick(const double &deltaTime);
};


class World : 
	public EngineSystem {
public:

	EntitySubsystem *entitySystem = nullptr;

	World();
	virtual void initialize() override;
	virtual void shutdown() override;
	virtual void beginPlay() override;
	virtual void endPlay() override;
	virtual void tick(const double &deltaTime) override;
	ESystemTickType tickType() override { return ESystemTickType::gameplay; }
};

#endif // _world