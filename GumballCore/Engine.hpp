#pragma once
#ifndef __engine
#define __engine
#include <iostream>
#include "Utils.hpp"
#include "EngineSystem.hpp"
#include "AssetManager.hpp"




class Engine : 
	public IEngineSystem,
	public Singleton<Engine>{

	map<string, IEngineSystem *> Systems;
public:
	Engine();
	~Engine();

	void Initialize() override;
	void Shutdown() override;
	void OnPlay() override;
	void OnEndplay() override;

	template<class T> T *getSystem();
};



#endif // !__engine
