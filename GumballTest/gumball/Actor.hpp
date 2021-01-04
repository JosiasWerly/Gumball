#pragma once
#ifndef _actor
#define _actor
#include <list>
#include "Actor.hpp"
#include "Transform.hpp"
#include "Drawables.hpp"
#include "Memory.hpp"

class ActorComponent;
class Actor : 
	public gObject{
protected:
	friend ActorComponent;
	std::set<ActorComponent*> components;
public:
	Transform transform;
	Actor& operator<<(ActorComponent* comp);
	Actor& operator>>(ActorComponent* comp);
	virtual void setEnable(bool enable);
};

class ActorComponent : 
	public gObject{
protected:
	friend Actor;
	Actor* owner;
	virtual void attachOwner(Actor* owner) {
		if (this->owner)
			detachOwner();
		this->owner = owner;
	}
	virtual void detachOwner() {
	}
public:
	Actor* getOwner() { return owner; }

	ActorComponent() {
	}
	virtual void tick() {
	}
};
class MeshComponent : 
	public ActorComponent {
	virtual void attachOwner(Actor* owner) {
		ActorComponent::attachOwner(owner);
		mesh.transform = &owner->transform;
		setEnable(true);
	}
	virtual void detachOwner() {
		ActorComponent::detachOwner();
		setEnable(false);
	}
public:
	MeshDrawable mesh;

	MeshComponent() {
	}
	void setEnable(bool enable){
		gObject::setEnable(enable);
		mesh.setEnableDraw(enable);
		//if(enable)
		//	RenderManager::instance().currentContext->operator<<(&mesh);
		//else
		//	RenderManager::instance().currentContext->operator>>(&mesh);
	}
};

#endif // !_actor