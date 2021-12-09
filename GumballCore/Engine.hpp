#pragma once
#ifndef __engine
#define __engine
#include "Patterns.hpp"
#include "EngineSystem.hpp"

#include <iostream>
#include <map>



class Engine : 
	public IEngineSystem,
	public Singleton<Engine>{

	map<string, IEngineSystem *> Systems;
public:
	Engine();
	~Engine();

	void initialize() override;
	void shutdown() override;
	void onPlay() override;
	void onEndplay() override;
};



#endif // !__engine
