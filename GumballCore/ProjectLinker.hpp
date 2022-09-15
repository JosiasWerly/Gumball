#pragma once
#ifndef _projectLinker
#define _projectLinker

#include <dllLoader/DynamicLibrary.hpp>
#include "Definitions.hpp"
#include "Engine.hpp"

class Project {
public:
	Project() {};
	virtual ~Project() {};
	virtual void onAttach(Engine &engine) {};
	virtual void onDettach() {};
	virtual void onTick() {};
};

class ProjectLinker {
	DynamicLibrary dll;
	std::time_t fileModifiedTime;

	Project *project = nullptr;	
public:
	void hotReload();
	void unload();
	Project *getProject();
	bool isProjectLoaded();
	bool hasNewVersion();
};


Extern GBCORE void GumballCoreEntryPoint();
typedef Project *(*FnxEntryPoint)();
#endif // !_projectLinker
