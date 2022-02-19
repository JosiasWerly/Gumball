#pragma once
#ifndef _engine
#define _engine
#include <list>

#include "Patterns.hpp"
#include "TimeStat.hpp"

//#include "EngineSystem.hpp"
//#include "AssetManager.hpp"
//#include "RenderSystem.hpp"
//#include "EditorOverlay.hpp"
//#include "Object.hpp"
//#include "Event.hpp"




class EditorOverlay;
class IEngineSystem;
class ProjectLinker;

class Engine : 
	public Singleton<Engine>{

	friend int main(int, char *[]);

	TimeStat timeStats;
	EditorOverlay *editor;
	std::list<IEngineSystem*> systems, tickingSystems;
	ProjectLinker* project;
	
	Engine();
	~Engine();
public:

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