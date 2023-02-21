#include "Engine.hpp"
#include "Subsystem.hpp"
#include "AssetManager.hpp"
#include "RenderSystem.hpp"
#include "SceneOverlay.hpp"
#include "WidgetOverlay.hpp"
#include "ProjectLinker.hpp"
#include "World.hpp"
#include "EnviromentVariables.hpp"

#include "Subsystem.hpp"
#include <iostream>
#include <string>
using namespace std;

Engine::Engine() {
	systemSeer = new SystemOverseer;

	project = nullptr;
	projectLinker = new ProjectLinker;
	renderSystem = systemSeer->addSystem<RenderSystem>();
	assetSystem = systemSeer->addSystem<AssetsSystem>();
	inputSystem = systemSeer->addSystem<InputSystem>();
	world = systemSeer->addSystem<World>();
}
Engine::~Engine() {
}
Inline void Engine::hotReload() {
	if (projectLinker->hasNewVersion()) {
		systemSeer->endPlay();
		if (project) {
			project->detached();
			delete project;
			project = nullptr;
		}
		if (project = projectLinker->load()) {
			project->attached();
			systemSeer->beginPlay();
		}
	}
}
void Engine::args(int argc, char *argv[]) {
	auto e = Enviroment::setInstance(new Enviroment);
	e->applicationPath = argv[0];
	e->applicationDir = e->applicationPath.substr(0, e->applicationPath.find_last_of("\\")) + "\\";
	e->resourcePath = e->applicationDir + "\\res\\";
	e->contentPath = e->applicationDir + "\\content\\";
}
void Engine::tick() {
	systemSeer->initialize();
	assetSystem->loadFromFolder(Enviroment::instance()->getResourcePath());

	auto widget = dynamic_cast<WidgetOverlay*>(renderSystem->getLayer("widget"));
	auto scene = dynamic_cast<SceneOverlay *>(renderSystem->getLayer("scene"));

	/// crappy way of controlling the play and stop button
	bool isPlaying = false;
	UI::Canvas win;
	UI::Text txt;
	UI::Button bt;
	bt.text = ">";
	(*widget) << &win;
	win << &txt << &bt;	
	//////////////////////////////////////////////////////////////////

	while (true) {
		hotReload();
		
		timeStats.capture();
		const double& deltaTime = timeStats.getDeltaTime();
		
		if (isPlaying) {
			systemSeer->tick<ESystemTickType::gameplay>(deltaTime);
			renderSystem->mainWindow.setTitle(to_string(timeStats.getFPS()));
		}
		else 
			systemSeer->tick<ESystemTickType::editor>(deltaTime);


		if (bt.isClicked()) {
			if (isPlaying) {
				isPlaying = false;
				systemSeer->endPlay();
				bt.text = ">";
			}
			else {
				isPlaying = true;
				systemSeer->beginPlay();
				bt.text = "||";
			}
		}
	}
	systemSeer->endPlay();
	systemSeer->shutdown();
}
