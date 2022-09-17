#pragma once
#ifndef _projectLinker
#define _projectLinker

#include <dllLoader/DynamicLibrary.hpp>

class Project {
public:
	Project() {}
	virtual ~Project() {}
	virtual void attached() {}
	virtual void detached() {}
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
