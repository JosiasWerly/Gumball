#include <iostream>
#include <filesystem>

#include "Script.hpp"
#include "InputManager.hpp"


void ProjectManager::attach(string filePath) {
	currentProject.dllProject.setup(filePath, "project");
}
void ProjectManager::load() {
	std::filesystem::copy(currentProject.dllProject.getPath(), "res\\project\\project.dll", std::filesystem::copy_options::recursive);
	if (currentProject.isValid())
		currentProject.endPlay();
	currentProject.dllProject.reload();
	currentProject.bindFunctions();
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