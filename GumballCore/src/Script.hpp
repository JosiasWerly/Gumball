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
#include <filesystem>


typedef void (*FnxProject)(void);

struct Project {
	FnxProject beginPlay;
	FnxProject endPlay;
	FnxProject tick;
	bool validFunctions() {
		return beginPlay && endPlay && tick;
	}
};

class ProjectManager {
	DynamicLibraryManager dllManager;
	DynamicLibrary* dllProject;
	
	bool updateReferences() {
		if (dllProject) {
			currentProject = {
				dllProject->getFunc<FnxProject>("beginPlay"),
				dllProject->getFunc<FnxProject>("endPlay"),
				dllProject->getFunc<FnxProject>("tick")
			};
			return currentProject.validFunctions();
		}
		return false;
	}
public:
	Project currentProject;
	void attach(string filePath) {
		dllManager.load(filePath, "p");
		dllProject = dllManager["p"];
		if (dllProject)
			updateReferences();
	}
	void reload() {
		dllManager.reload(dllProject->name);
		dllProject = dllManager["p"];
		if (dllProject)
			updateReferences();
		
	}
};


//std::filesystem::last_write_time
//Extern Project* instantiateProject();
#endif // !_script
