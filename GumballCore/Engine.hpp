#pragma once
#ifndef _engine
#define _engine
#include <list>

#include "Patterns.hpp"
#include "TimeStat.hpp"

class SystemOverseer;
class EngineSystem;
class EditorOverlay;
class ProjectLinker;
class RenderSystem;
class AssetsSystem;
class InputSystem;
class TimeStat;
class Project;
class World;


class GBCORE Engine : 
	public Singleton<Engine>{
private:
	friend int main(int, char *[]);

	TimeStat timeStats;
	Project *project;
	ProjectLinker *projectLinker;	
	SystemOverseer *systemSeer;
	
	Engine();
	~Engine();
	
	Inline void hotReload();
public:
	void args(int argc, char *argv[]);
	void tick();

	RenderSystem *renderSystem;
	AssetsSystem *assetSystem;
	InputSystem *inputSystem;
	World *world;
};


#endif // !_engine