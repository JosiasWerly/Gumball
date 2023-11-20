#pragma once
#ifndef _world
#define _world

//#include "Object.hpp"
//#include "Definitions.hpp"
#include "Subsystem.hpp"
#include "Var.hpp"

#include <deque>
#include <list>
using namespace std;

class GameObject;
class GameScene;
class World;

enum class EGameObjectState {
	spawn, nominal, destroy
};
class GBCORE GameObject {
	friend class GameScene;
private:
	EGameObjectState state;
	bool tickEnable = false;
	float tickInterval = 0;
protected:
public:
	GameObject();
	virtual ~GameObject();
	virtual void beginPlay();
	virtual void endPlay();
	virtual void tick(const double &deltaTime);

	void setTickEnable(bool newState);
	Inline bool getTickEnable() const { return tickEnable; }
	void setTickInterval(float newValue);
	Inline bool getTickInterval() const { return tickInterval; }

	const bool isValid() const { return this && state == EGameObjectState::nominal; }

	void *operator new(unsigned long long sz);
};

class GBCORE GameScene {
	friend class GameObject;
private:
	deque<GameObject*> entities;
	deque<GameObject*> toBegin, toEnd, toTick;
	list<GameObject*> toChangeTick;

	Inline void __erase(deque<GameObject *> &dq, GameObject *trg) {
		auto found = std::find(dq.begin(), dq.end(), trg);
		if (found != dq.end()) {
			dq.erase(found);
		}
	}
public:
	void unload();
	void root(GameObject *go);
	void unRoot(GameObject *go);
	void changeTick(GameObject *go);
	Inline void tick(const double &deltaTime);
};


Extern GBCORE void destroy(GameObject *trg);

class World : 
	public EngineSystem {
public:
	GameScene scene;

	World();
	virtual void initialize() override;
	virtual void shutdown() override;
	virtual void beginPlay() override;
	virtual void endPlay() override;
	virtual void tick(const double &deltaTime) override;
	ESystemTickType tickType() override { return ESystemTickType::gameplay; }
};

#endif // _world