#pragma once
#ifndef _projectLinker
#define _projectLinker

#include "Definitions.hpp"
#include "Engine.hpp"

typedef void (*ProjectFnx)(Engine &Engine);

class ProjectLinker {
public:
	ProjectLinker() = default;
	virtual ~ProjectLinker() {}
	//use signals, better than funcion override
};


Extern DLL void OnProjectInit(ProjectLinker &project);

#endif // !_projectLinker
