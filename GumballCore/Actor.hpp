#pragma once
#ifndef _actor
#define _actor

#include "World.hpp"
#include "Math.hpp"
#include "SceneOverlay.hpp"

class Actor;
class GBCORE ActorComponent :
	public GameObject {
	friend class Actor;
private:
	bool isActive = true;
	
protected:
	Actor *owner;
	
	virtual void activate();
	virtual void deactivate();
	virtual void newOwner(Actor* newOwner, Actor* oldOwner);
public:
	ActorComponent();
	virtual ~ActorComponent();
	virtual void beginPlay();
	virtual void endPlay();
	virtual void tick(const double &deltaTime);


	void setActive(bool newActive);
	bool getActive() const { return isActive; };
	Actor *getOwner() const { return owner; }
};

class GBCORE MeshComponent :
	public ActorComponent {
	DrawHandle draw;
protected:
	void activate() override;
	void deactivate() override;
	void newOwner(Actor *newOwner, Actor *oldOwner) override;
public:
	MeshComponent();
	virtual ~MeshComponent();
};
class GBCORE SpriteComponent :
	public ActorComponent {
public:
	SpriteComponent() {
	}
	virtual ~SpriteComponent() {
	}
};
class GBCORE CameraComponent :
	public ActorComponent {
	ViewHandle view;
protected:
	void activate() override;
	void deactivate() override;
	void newOwner(Actor *newOwner, Actor *oldOwner) override;
public:
	CameraComponent();
	virtual ~CameraComponent();

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
	template<class t> t *findComponent();
	const list<ActorComponent *> &getComponents() { return components; }
};

#endif // _actor