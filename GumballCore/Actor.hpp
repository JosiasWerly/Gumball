#pragma once
#ifndef _actor
#define _actor

#include "World.hpp"
#include "Math.hpp"

class GBCORE Component :
	public GameObject {
	friend class Actor;
private:
	Actor *owner;
public:
	Component();
	virtual ~Component();
};

class GBCORE Actor :
	public GameObject {
	friend class World;
private:
	list<Component *> components;
public:
	Transform transform;

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

#endif // _actor