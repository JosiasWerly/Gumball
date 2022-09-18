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


enum class EGameObjectState {
	nominal, begining, ending
};
class GBCORE GameObject {
	EGameObjectState state;
	string name;
public:
	GameObject();
	GameObject(string name);

	virtual ~GameObject();
	virtual void beginPlay();
	virtual void endPlay();
	virtual void tick(const double &deltaTime);	


	string getName();
	void setName(string name);
	const bool isValid() const { return state == EGameObjectState::nominal; }
};

class GBCORE EntitySubsystem {
	friend class GameObject;
	list<Var<GameObject>> entities;
	list<Var<GameObject> *> toBegin, toEnd, toTick;
public:
	void unload();
	void add(GameObject *go);
	void del(GameObject *go);
	Inline void tick(const double &deltaTime);
};

class World : 
	public Subsystem {

public:
	EntitySubsystem *entitySystem;

	World();
	virtual void initialize() override;
	virtual void shutdown() override;
	virtual void beginPlay() override;
	virtual void endPlay() override;
	virtual void tick(const double &deltaTime) override;
};

class GBCORE Component : 
	public GameObject {
	friend class Actor;
private:
	Actor *owner;
public:
	Component();
	~Component();
};

class GBCORE Actor :
	public GameObject {
	friend class World;
private:
	list<Component*> components;
public:
	static void *operator new(unsigned long long sz);
	static void operator delete(void *ptr);
	
	Actor();
	virtual ~Actor();
	virtual void beginPlay();
	virtual void endPlay();
	virtual void tick(const double &deltaTime);

	void addComponent(Component *comp);
	void delComponent(Component *comp);
	Component *findComponent(string name);
	template<class t> t *findComponent();
	const list<Component *> &getComponents() { return components; }
};


#endif // _world
