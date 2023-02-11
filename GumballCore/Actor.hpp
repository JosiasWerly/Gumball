#pragma once
#ifndef _actor
#define _actor

#include "World.hpp"
#include "Math.hpp"

class GBCORE ActorComponent :
	public GameObject {
	friend class Actor;
protected:
	Actor *owner;
public:
	ActorComponent();
	virtual ~ActorComponent();

};

class GBCORE Actor :
	public GameObject {
	friend class World;
private:
	list<ActorComponent *> components;
public:
	Transform transform;

	Actor();
	virtual ~Actor();
	virtual void beginPlay();
	virtual void endPlay();
	virtual void tick(const double &deltaTime);

	void addComponent(ActorComponent *comp);
	void delComponent(ActorComponent *comp);
	ActorComponent *findComponent(string name);
	template<class t> t *findComponent();
	const list<ActorComponent *> &getComponents() { return components; }
};

#endif // _actor