#pragma once
#ifndef _worldsystem
#define _worldsystem

#include "SystemController.hpp"
#include "Var.hpp"

#include <deque>
#include <list>
using namespace std;

class GameObject;
class WorldEntityController;
class WorldSystem;

enum class EGameObjectState {
	spawn, nominal, destroy
};
class GBCORE GameObject {
	friend class WorldEntityController;
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

class GBCORE WorldEntityController {
	friend class WorldSystem;
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
	void tick(const double &deltaTime);
};

Extern GBCORE void destroy(GameObject *trg);
class WorldSystem : public System {
public:
	WorldEntityController entityController;

	WorldSystem();
	virtual void initialize() override;
	virtual void shutdown() override;
	virtual void beginPlay() override;
	virtual void endPlay() override;
	virtual void tick(const double &deltaTime) override;
	ESystemTickType tickType() override { return ESystemTickType::gameplay; }
};

#endif // _worldsystem