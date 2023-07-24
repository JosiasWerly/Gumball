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
	Project *project = nullptr;
	DynamicLibrary dll;
	std::time_t fileModifiedTime;
public:
	bool hasNewVersion();
	void unload();
	Project *load();
	Project *getProject() const { return project; }
};


Extern GBCORE void GumballCoreEntryPoint();
typedef Project *(*FnxEntryPoint)();
#endif // !_projectLinker
