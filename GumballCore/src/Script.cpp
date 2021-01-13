#include <iostream>
#include <filesystem>

#include "Script.hpp"
#include "InputManager.hpp"


void ProjectManager::attach(string filePath) {
	projectPath = filePath;	
}
void ProjectManager::load() {
	std::filesystem::copy(this->projectPath, "res\\project\\project.dll", std::filesystem::copy_options::recursive);
	if (dllManager["project"])
		currentProject.endPlay();
	dllManager.free("project");
	dllManager.load("res\\project\\project.dll", "project");
	currentProject.attach(dllManager["project"]);
	hasSetup = false;
}
void ProjectManager::tick() {
	if (Input::onPressed(eKeyboard::UP))
		load();
	else if (currentProject.isValid()) {
		if (!hasSetup) {
			hasSetup = true;
			currentProject.beginPlay();
		}
		currentProject.tick();
	}
}

//
//Extern Project* instantiateProject();