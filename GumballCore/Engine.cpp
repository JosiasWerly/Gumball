#include "Engine.hpp"
#include "EngineSystem.hpp"
#include "AssetManager.hpp"
#include "RenderSystem.hpp"
#include "SceneOverlay.hpp"
#include "WidgetOverlay.hpp"
#include "ProjectLinker.hpp"
#include "EnviromentVariables.hpp"

#include <iostream>
#include <string>


using namespace std;

Engine::Engine() {	
	projectLinker = new ProjectLinker;
	assetSystem = new AssetsSystem;
	renderSystem = new RenderSystem;
	inputSystem = new InputSystem;

	systems.push_back(assetSystem);
	systems.push_back(renderSystem);
	systems.push_back(inputSystem);

	tickingSystems.push_back(renderSystem);
	tickingSystems.push_back(inputSystem);
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
	//if (!project) {
	//	if (projectLinker->hasNewVersion()) {
	//		endPlay();
	//		project->shutdown();
	//		systems.remove(project);
	//		tickingSystems.remove(project);
	//		delete project;
	//		project = nullptr;
	//	}
	//	if (project = projectLinker->load()) {
	//		systems.push_back(project);
	//		tickingSystems.push_back(project);
	//		project->initialize();
	//		beginPlay();
	//	}
	//}
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
	int i = 0;

	while (true) {
		hotReload();
		timeStats.capture();
		TimeStat debugTimeStats;
		for (auto &s : tickingSystems) {
			debugTimeStats.capture();
			s->tick((float)timeStats.getDeltaTime());
			debugTimeStats.capture();
		}
	}

	endPlay();
	shutdown();
}






//void Engine::tick() {
//	initialize();
//	assetSystem->loadAssetsFromFolder(Enviroment::instance()->getResourcePath());
//
//	auto widget = dynamic_cast<WidgetOverlay *>(getSystem<RenderSystem>()->getLayer("editor"));
//	auto scene = dynamic_cast<SceneOverlay *>(getSystem<RenderSystem>()->getLayer("scene"));
//
//	auto v = new View;
//	v->viewMode.setProjectionPerspective();
//	v->transform.position.z = -10;
//	scene->pushView(v);
//
//	UI::Canvas win;
//	UI::Text txt;
//	UI::Button bt;
//	(*widget) << &win;
//	win << &txt << &bt;
//	int i = 0;
//
//	while (true) {
//
//
//
//		//{
//		//	if (projectLinker->hasNewVersion()) {
//		//		if (Project *proj = projectLinker->getProject()) {
//		//			endPlay();
//		//			proj->onDettach();
//		//			projectLinker->unload();
//		//		}
//		//		projectLinker->hotReload();
//		//		if (Project *proj = projectLinker->getProject()) {
//		//			proj->onAttach();
//		//			beginPlay();
//		//		}
//		//	}
//		//}
//
//
//		static string names[] = { "render", "input", "object" };
//		timeStats.capture();
//		//editor->msStats["fps"] = timeStats.getFPS();
//
//		int id = 0;
//		TimeStat debugTimeStats;
//		for (auto &s : tickingSystems) {
//			debugTimeStats.capture();
//			s->tick((float)timeStats.getDeltaTime());
//			debugTimeStats.capture();
//			//editor->msStats[names[id++]] = debugTimeStats.getMS();
//		}
//	}
//
//	endPlay();
//	shutdown();
//}















//View v;
//v.viewMode.setProjectionPerspective();
//v.transform.position.z = -10;
//scene->pushView(&v);
//
//DrawInstance a;
//a.setMesh("cube");
//a.setTexture("logo");
//a.transform.position.x = -1;
//scene->pushDrawInstance(&a);
//
//
//DrawInstance b;
//b.setMesh("cube");
//b.setTexture("scotty");
//b.transform.position.x = 1;
//scene->pushDrawInstance(&b);
//
//
//if (inputSystem->isKeyDown(Input::EKeyCode::W))
//a.transform.position += a.transform.rotator.forward() * .5;
//else if (inputSystem->isKeyDown(Input::EKeyCode::S))
//a.transform.position -= a.transform.rotator.forward() * .5;
//
//if (inputSystem->isKeyDown(Input::EKeyCode::D))
//a.transform.position += a.transform.rotator.right() * .5;
//else if (inputSystem->isKeyDown(Input::EKeyCode::A))
//a.transform.position -= a.transform.rotator.right() * .5;
//
//if (inputSystem->isKeyDown(Input::EKeyCode::UP))
//a.transform.rotator.rotate(1, 0, 0);
//else if (inputSystem->isKeyDown(Input::EKeyCode::DOWN))
//a.transform.rotator.rotate(-1, 0, 0);
//
//if (inputSystem->isKeyDown(Input::EKeyCode::LEFT))
//a.transform.rotator.rotate(0, 0, -1);
//else if (inputSystem->isKeyDown(Input::EKeyCode::RIGHT))
//a.transform.rotator.rotate(0, 0, 1);