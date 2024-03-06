#pragma once
#ifndef _engine
#define _engine
#include <list>

#include "Patterns.hpp"
#include "TimeStat.hpp"

class SystemController;
class ProjectLinker;
class TimeStat;

class EditorSystem;
class RenderSystem;
class AssetsSystem;
class InputSystem;
class WorldSystem;


class GBCORE Engine : 
	public Singleton<Engine>{
private:
	friend int main(int, char *[]);

	TimeStat timeStats;
	ProjectLinker *projectLinker;	
	SystemController *systemController;
	
	Engine();
	~Engine();	
public:
	void args(int argc, char *argv[]);
	void tick();

	//self explanatory names
	RenderSystem *renderSystem;
	AssetsSystem *assetSystem;
	InputSystem *inputSystem;
	WorldSystem *worldSystem;
	EditorSystem *editorSystem;
};


#endif // !_engine