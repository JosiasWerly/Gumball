#pragma once
#ifndef __engine
#define __engine
#include "Patterns.hpp"
#include "EngineSystem.hpp"
#include "Event.hpp"
#include "AssetManager.hpp"
#include "RenderSystem.hpp"
#include "EditorOverlay.hpp"
#include "Object.hpp"
#include "TimeStat.hpp"

#include <iostream>
#include <map>

class Engine :
	public Singleton<Engine> {

	TimeStat timeStats;
	EditorOverlay *editor;

	list<IEngineSystem*> systems, tickingSystems;
	
	//for now the project is being loaded directly from here.
	class ProjectLinker* project;
public:
	RenderSystem &renderSystem = RenderSystem::instance();
	AssetsSystem &assetSystem = AssetsSystem::instance();
	InputSystem &inputSystem = InputSystem::instance();
	ObjectSystem &objectSystem = ObjectSystem::instance();


	Engine();
	~Engine();
	
	void args(int argc, char *argv[]);
	void tick();
};



#endif // !__engine
