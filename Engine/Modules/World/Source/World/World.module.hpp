#pragma once
#ifndef _worldmodule
#define _worldmodule

#include <deque>
#include <list>
using namespace std;

class GameObject;
class WorldEntityController;
class WorldSystem;

enum class EGameObjectState {
	spawn, nominal, destroy
};
class GMODULE GameObject {
	friend class WorldEntityController;

private:
	EGameObjectState state;
	bool tickEnable = false;
	float tickInterval = 0;

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

class GMODULE WorldEntityController {
	friend class WorldSystem;

private:
	deque<GameObject *> entities;
	deque<GameObject *> toBegin, toEnd, toTick;
	list<GameObject *> toChangeTick;

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

class GMODULE Level {
public:
};

Extern GMODULE void destroy(GameObject *trg);
class GMODULE WorldModule : public TModule<WorldModule> {
public:
	WorldEntityController entityController;

	WorldModule();
	void shutdown() override;
	void beginPlay() override;
	void endPlay() override;
	void tick(const double &deltaTime) override;
	EModuleTickType tickType() override { return EModuleTickType::gameplay; }
};

#endif // _worldmodule