#pragma once
#ifndef _actor
#define _actor
#include <list>
#include "Actor.hpp"
#include "Transform.hpp"
#include "Drawables.hpp"
#include "Memory.hpp"

class Actor : 
	public gObject {
	std::list<class ActorComponent*> components;
public:
	Transform transform;

	void pushComponent(class ActorComponent* comp) {
		components.push_back(comp);
	}
	void popComponent(class ActorComponent* comp) {
		components.remove(comp);
	}
	virtual void enable() {
		gObject::enable();
	}
	virtual void disable() {
		gObject::disable();
	}
	virtual void constructor() {
	}
	virtual void destructor() {
	}
	virtual void tick() {
	}
};

class ActorComponent :
	public gObject{
public:
	Actor* owner;
	ActorComponent(Actor* owner) {
		this->owner = owner;
	}
	virtual void constructor() {
	}
	virtual void destructor() {
	}
	virtual void tick() {
	}
};
class MeshComponent : 
	public ActorComponent {
public:
	using ActorComponent::ActorComponent;
	MeshDrawable mesh;
	MeshComponent(Actor* owner) : 
		ActorComponent(owner){		
		mesh.transform = &owner->transform;
	}
};

#endif // !_actor