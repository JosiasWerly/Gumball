#pragma once
#ifndef _engine
#define _engine
#include <list>

#include "Patterns.hpp"
#include "TimeStat.hpp"

class IEngineSystem;
class EditorOverlay;
class ProjectLinker;
class RenderSystem;
class AssetsSystem;
class InputSystem;
class TimeStat;
class Project;


class GBCORE Engine : 
	public Singleton<Engine>{
private:
	friend int main(int, char *[]);

	TimeStat timeStats;
	std::list<IEngineSystem*> systems, tickingSystems;
	ProjectLinker *projectLinker;
	
	Engine();
	~Engine();

	Inline void endPlay() const;
	Inline void beginPlay() const;
	Inline void shutdown() const;
	Inline void initialize() const;
	Inline void hotReload();
public:
	Project *project;
	RenderSystem *renderSystem;
	AssetsSystem *assetSystem;
	InputSystem *inputSystem;

	void args(int argc, char *argv[]);
	void tick();

	template<class T>
	T *getSystem() {
		for (auto s : systems) {
			if (T *sys = reinterpret_cast<T *>(s))
				return sys;
		}
		return nullptr;
	}
};


#endif // !_engine