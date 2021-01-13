#pragma once
#ifndef _script
#define _script

#ifdef expDLL
#define DLL __declspec(dllexport)
#else
#define DLL __declspec(dllimport)
#endif
#define Extern extern "C" DLL


/// GumballSide
#include "dllLoader/dllLoader.hpp"

typedef void (*FnxProject)(void);
class Project {
public:
	DynamicLibrary* dllProject;
	FnxProject beginPlay;
	FnxProject endPlay;
	FnxProject tick;


	void attach(DynamicLibrary* newDll) {
		dllProject = newDll;
		if (dllProject) {			
			beginPlay = dllProject->getFunc<FnxProject>("beginPlay");
			endPlay = dllProject->getFunc<FnxProject>("endPlay");
			tick = dllProject->getFunc<FnxProject>("tick");
		}
	}
	bool isValid() {
		return beginPlay && endPlay && tick;
	}
};
class ProjectManager {	
	DynamicLibraryManager dllManager;
public:
	bool hasSetup;
	string projectPath;
	Project currentProject;


	void attach(string filePath); 
	void load();
	void tick();	
};  

#endif // !_script
