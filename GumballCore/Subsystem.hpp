#pragma once
#ifndef _enginesystem
#define _enginesystem

#include <list>

class Engine;
class SystemOverseer;

enum class ESystemTickType {
	disable,
	editor,
	gameplay,
	all
};
class GBCORE EngineSystem {
protected:
	friend SystemOverseer;
	friend Engine;

	EngineSystem();
	virtual ~EngineSystem();
	virtual void initialize();
	virtual void shutdown();
	virtual void beginPlay();
	virtual void endPlay();
	virtual void tick(const double &deltaTime);
	virtual ESystemTickType tickType();
};

class GBCORE SystemOverseer {
	friend Engine;
private:
	std::list<EngineSystem *> systems;
	std::list<EngineSystem *> editorTick, gameplayTick;
	
	SystemOverseer();
protected:
	Inline void beginPlay() const;
	Inline void endPlay() const;
	Inline void shutdown() const;
	Inline void initialize() const;

	template<ESystemTickType> void tick(const double &deltaTime) {
	}
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
#endif // !_enginesystem
