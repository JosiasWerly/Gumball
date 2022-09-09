#pragma once
#ifndef _engine
#define _engine
#include <list>

#include "Patterns.hpp"
#include "TimeStat.hpp"

class EditorOverlay;
class IEngineSystem;
class ProjectLinker;

class RenderSystem;
class AssetsSystem;
class InputSystem;


class Engine : 
	public Singleton<Engine>{

	friend int main(int, char *[]);

	TimeStat timeStats;
	std::list<IEngineSystem*> systems, tickingSystems;
	ProjectLinker* project;
	
	Engine();
	~Engine();
public:
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