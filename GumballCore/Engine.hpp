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
	private IEngineSystem,
	public Singleton<Engine> {

	TimeStat timeStats;
	EditorOverlay *editor;

	list<IEngineSystem*> systems, tickingSystems;
public:
	RenderSystem &renderSystem = RenderSystem::instance();
	AssetsSystem &assetSystem = AssetsSystem::instance();
	InputSystem &inputSystem = InputSystem::instance();
	ObjectSystem &objectSystem = ObjectSystem::instance();

	Engine();
	~Engine();

	void initialize() override;
	void shutdown() override;
	void tick(float deltaTime) override;

	void onPlay() override;
	void onEndplay() override;
};



#endif // !__engine
