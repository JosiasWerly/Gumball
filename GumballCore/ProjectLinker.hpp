#pragma once
#ifndef _projectLinker
#define _projectLinker

#include <dllLoader/DynamicLibrary.hpp>
#include "Definitions.hpp"
#include "Engine.hpp"


class ProjectLinker {
	DynamicLibrary dll;
	std::time_t fileModifiedTime;

	typedef void(*FnxOnProjectAttached)(Engine &engineRef);
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

//TODO: perhaps I should use inherance for extending this points
Extern void OnProjectAttached(Engine &engineRef);
Extern void OnProjectDettached();

#endif // !_projectLinker
