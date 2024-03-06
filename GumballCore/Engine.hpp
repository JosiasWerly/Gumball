#pragma once
#ifndef _engine
#define _engine
#include <list>

#include "Patterns.hpp"
#include "TimeStat.hpp"

class SystemOverseer;
class ProjectLinker;
class RenderSystem;
class AssetsSystem;
class InputSystem;
class ConsoleSystem;
class TimeStat;
class World;


class GBCORE Engine : 
	public Singleton<Engine>{
private:
	friend int main(int, char *[]);

	TimeStat timeStats;
	ProjectLinker *projectLinker;	
	SystemOverseer *systemSeer;
	
	Engine();
	~Engine();	
public:
	void args(int argc, char *argv[]);
	void tick();

	RenderSystem *renderSystem;
	AssetsSystem *assetSystem;
	InputSystem *inputSystem;
	ConsoleSystem *consoleSystem;
	World *world;
};


#endif // !_engine