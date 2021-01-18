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
	DynamicLibrary dllProject;
	FnxProject beginPlay, endPlay, tick;
	void bindFunctions() {
		if (dllProject.isValid()) {
			beginPlay = dllProject.getFunc<FnxProject>("beginPlay");
			endPlay = dllProject.getFunc<FnxProject>("endPlay");
			tick = dllProject.getFunc<FnxProject>("tick");
		}
	}
	bool isValid() {
		return beginPlay && endPlay && tick;
	}
};
class ProjectManager {
public:
	bool hasSetup;
	Project currentProject;

	void attach(string filePath); 
	void load();
	void tick();	
};  

#endif // !_script
