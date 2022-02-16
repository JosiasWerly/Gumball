#pragma once
#ifndef _projectLinker
#define _projectLinker

#include <dllLoader/DynamicLibrary.hpp>
#include "Definitions.hpp"
#include "Engine.hpp"


class ProjectLinker {
	DynamicLibrary dll;
	std::time_t fileModifiedTime;

	typedef void(*FnxOnProjectAttached)(ProjectLinker &project);
	typedef void(*FnxOnProjectDettached)();

	FnxOnProjectAttached onAttached;
	FnxOnProjectDettached onDettached;

public:
	string dllPath;
	string enginePath;

	void load();
	bool hasToLoad();
	Inline bool isLoaded() {
		return dll.isLoaded();
	}
};


Extern void OnProjectAttached(ProjectLinker &project);
Extern void OnProjectDettached();

#endif // !_projectLinker
