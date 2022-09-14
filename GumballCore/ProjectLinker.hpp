#pragma once
#ifndef _projectLinker
#define _projectLinker

#include <dllLoader/DynamicLibrary.hpp>
#include "Definitions.hpp"
#include "Engine.hpp"

class Project {
public:
	Project();
	virtual ~Project();
	virtual void onAttach(Engine &engine);
	virtual void onDettach();
	virtual void onTick();
};

class ProjectLinker {
	DynamicLibrary dll;
	std::time_t fileModifiedTime;

	string dllPath;
	string enginePath;
public:
	void setup(string dllPath, string enginePath);
	
	Project *linkerTargetInstance();
	bool isNewLinkerAvailable();
	Inline bool hasLinker() { return dll.isLoaded(); }
};


Extern GBCORE void GumballCoreEntryPoint();
typedef Project *(*FnxEntryPoint)();
#endif // !_projectLinker
