#pragma once
#ifndef _project
#define _project

#include <Gumball/Framework/DynamicLib.hpp>
#include <chrono>

namespace Plugin {

class GENGINE Project {
public:
	Project() {}
	virtual ~Project() {}
	virtual void Attached() {}
	virtual void Detached() {}
};

class GENGINE ProjectLinker {
	Project *project = nullptr;
	DynamicLibrary dll;
	std::time_t fileModifiedTime;

public:
	bool HasNewVersion();
	void Unload();
	Project *Load();
	Project *Get() const { return project; }
};

};

Extern GENGINE void GumballCoreEntryPoint();
typedef Plugin::Project *(*FnxEntryPoint)();
#endif // !_project