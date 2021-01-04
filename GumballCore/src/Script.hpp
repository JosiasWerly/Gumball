#pragma once
#ifndef _script
#define _script

#ifdef expDLL
#define DLL __declspec(dllexport)
#else
#define DLL __declspec(dllimport)
#endif
#define Extern extern "C" DLL


typedef void (*FnxNewProject)(void);

/// GumballSide
#include "dllLoader/dllLoader.hpp"
#include <filesystem>

struct Project{

};
class ProjectManager {
	DynamicLibraryManager dllManager;
	DynamicLibrary* dllProject;
	FnxNewProject fnxInstance;
	Project* project;
public:
	void attach(string filePath) {
		dllManager.load(filePath, "p");
		dllProject = dllManager["p"];
		if (dllProject) {
			fnxInstance = dllProject->getFunc<FnxNewProject>("instantiateProject");
			if (fnxInstance) {
				project = fnxInstance();
			}
		}
	}
	void reload() {		
		delete project;
		project = nullptr;
		dllManager.reload(dllProject->name);
		dllProject = dllManager["p"];
		if (dllProject) {
			fnxInstance = dllProject->getFunc<FnxNewProject>("instantiateProject");
			if (fnxInstance) {
				project = fnxInstance();
			}
		}
	}
	void shutdown() {
		if (project)
			project->shutdown();
	}
	void setup() {
		if (project)
			project->setup();
	}
	void tick() {
		if (project)
			project->tick();
	}
};


//std::filesystem::last_write_time
//Extern Project* instantiateProject();
#endif // !_script
