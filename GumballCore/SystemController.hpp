#pragma once
#ifndef _systemcontroller
#define _systemcontroller

#include <list>

enum class ESystemTickType {
	none,
	editor,
	gameplay,
	all
};

class GBCORE System {
protected:
	friend class SystemController;
	friend class Engine;

	System() = default;
	virtual ~System() = default;
	virtual void initialize();
	virtual void lateInitialize();
	virtual void shutdown();
	virtual void beginPlay();
	virtual void endPlay();
	virtual void tick(const double &deltaTime);
	virtual ESystemTickType tickType();
};

class GBCORE SystemController {
	friend class Engine;
private:
	std::list<System *> systems;
	std::list<System *> editorTick, gameplayTick;
	
	SystemController();
protected:
	void initialize() const;
	void lateInitialize() const;
	void shutdown() const;
	void beginPlay() const;
	void endPlay() const;

	template<ESystemTickType> void tick(const double &deltaTime) = delete;
	template<> void tick<ESystemTickType::editor>(const double &deltaTime) {
		for (auto &s : editorTick)
			s->tick(deltaTime);
	}
	template<> void tick<ESystemTickType::gameplay>(const double &deltaTime) {
		for (auto &s : gameplayTick)
			s->tick(deltaTime);
	}
public:

	template<class T>
	T *addSystem() {
		T *sys = getSystem<T>();
		if (!sys) {
			sys = new T;
			systems.push_back(sys);
			ESystemTickType tick = sys->tickType();
			switch (tick) {
				case ESystemTickType::editor:
					editorTick.push_back(sys);
					break;
				case ESystemTickType::gameplay:
					gameplayTick.push_back(sys);
					break;
				case ESystemTickType::all:
					editorTick.push_back(sys);
					gameplayTick.push_back(sys);
					break;
			}
		}
		return sys;
	}

	template<class T>
	T *getSystem() {
		for (auto s : systems) {
			if (T *sys = dynamic_cast<T *>(s))
				return sys;
		}
		return nullptr;
	}
};
#endif // !_systemcontroller
