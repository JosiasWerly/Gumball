#pragma once
#ifndef _project
#define _project

#include "DynamicLib.hpp"
#include <chrono>

class GENGINE Project {
public:
	Project() {}
	virtual ~Project() {}
	virtual void attached() {}
	virtual void detached() {}
};

class GENGINE ProjectTarget {
	Project *project = nullptr;
	DynamicLibrary dll;
	std::time_t fileModifiedTime;

public:
	bool hasNewVersion();
	void unload();
	Project *load();
	Project *getProject() const { return project; }
};


Extern GENGINE void GumballCoreEntryPoint();
typedef Project *(*FnxEntryPoint)();
#endif // !_project
