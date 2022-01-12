#pragma once
#ifndef __engine
#define __engine
#include "Patterns.hpp"
#include "EngineSystem.hpp"
#include "Event.hpp"

#include <iostream>
#include <map>



class Engine :
	private IEngineSystem,
	public Singleton<Engine> {

	list<IEngineSystem*> Systems, TickingSystems;
public:
	Engine();
	~Engine();

	void initialize() override;
	void shutdown() override;
	void tick() override;

	void onPlay() override;
	void onEndplay() override;
};



#endif // !__engine
