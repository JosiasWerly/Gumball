#pragma once
#ifndef _projectLinker
#define _projectLinker

#include <dllLoader/DynamicLibrary.hpp>
//#include "EngineSystem.hpp"

class Project /*: 
	public IEngineSystem*/ {
public:
	Project() {}
	virtual ~Project() {}
	virtual void initialize() {}
	virtual void shutdown() {}
	virtual void tick(float deltaTime) {}
	virtual void onPlay() {}
	virtual void onEndplay() {}
};

class ProjectLinker {
	DynamicLibrary dll;
	std::time_t fileModifiedTime;
public:
	bool hasNewVersion();
	void unload();
	Project *load();
};


Extern GBCORE void GumballCoreEntryPoint();
typedef Project *(*FnxEntryPoint)();
#endif // !_projectLinker
