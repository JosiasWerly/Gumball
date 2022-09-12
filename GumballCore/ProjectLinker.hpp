#pragma once
#ifndef _projectLinker
#define _projectLinker

#include <dllLoader/DynamicLibrary.hpp>
#include "Definitions.hpp"
#include "Engine.hpp"

class IProject;
class ProjectLinker {
	DynamicLibrary dll;
	std::time_t fileModifiedTime;

	string dllPath;
	string enginePath;
public:
	void setup(string dllPath, string enginePath);
	
	IProject* linkerTargetInstance();
	bool isNewLinkerAvailable();
	Inline bool hasLinker() { return dll.isLoaded(); }
};


class IProject {
protected:
	IProject() = default;
public:
	virtual ~IProject() {}
	virtual void onAttach(Engine &engine) = 0;
	virtual void onDettach() = 0;
};

//TODO: perhaps I should use inherance for extending this points
Extern IProject* OnProjectAttached();
typedef IProject *(*FnxOnProjectAttached)();

#endif // !_projectLinker
