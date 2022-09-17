#include "Engine.hpp"
#include "EngineSystem.hpp"
#include "AssetManager.hpp"
#include "RenderSystem.hpp"
#include "SceneOverlay.hpp"
#include "WidgetOverlay.hpp"
#include "ProjectLinker.hpp"
#include "World.hpp"
#include "EnviromentVariables.hpp"

#include <iostream>
#include <string>
using namespace std;

Engine::Engine() {	
	project = nullptr;
	projectLinker = new ProjectLinker;
	renderSystem = new RenderSystem;
	assetSystem = new AssetsSystem;
	inputSystem = new InputSystem;
	world = new World;

	systems.push_back(renderSystem);
	systems.push_back(assetSystem);
	systems.push_back(inputSystem);
	systems.push_back(world);

	tickingSystems.push_back(renderSystem);
	tickingSystems.push_back(inputSystem);
	tickingSystems.push_back(world);
}
Engine::~Engine(){
}
Inline void Engine::endPlay() const {
	for (auto &s : systems)
		s->onEndplay();
}
Inline void Engine::beginPlay() const {
	for (auto &s : systems)
		s->onPlay();
}
Inline void Engine::shutdown() const {
	for (auto &s : systems)
		s->shutdown();
}
Inline void Engine::initialize() const {
	for (auto &s : systems)
		s->initialize();
}
Inline void Engine::hotReload() {
	if (projectLinker->hasNewVersion()) {
		endPlay();
		if (project) {
			project->shutdown();
			systems.remove(project);
			tickingSystems.remove(project);
			delete project;
			project = nullptr;
		}
		if (project = projectLinker->load()) {
			systems.push_back(project);
			tickingSystems.push_back(project);
			project->initialize();
			beginPlay();
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
	initialize();
	assetSystem->loadAssetsFromFolder(Enviroment::instance()->getResourcePath());

	auto widget = dynamic_cast<WidgetOverlay*>(getSystem<RenderSystem>()->getLayer("widget"));
	auto scene = dynamic_cast<SceneOverlay *>(getSystem<RenderSystem>()->getLayer("scene"));

	auto v = new View;
	v->viewMode.setProjectionPerspective();
	v->transform.position.z = -10;
	scene->pushView(v);

	UI::Canvas win;
	UI::Text txt;
	UI::Button bt;
	(*widget) << &win;
	win << &txt << &bt;

	while (true) {
		hotReload();
		
		timeStats.capture();
		for (auto &s : tickingSystems)
			s->tick(timeStats.getDeltaTime());
	}

	endPlay();
	shutdown();
}