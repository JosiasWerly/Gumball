#include "Engine.hpp"
#include "SystemController.hpp"
#include "SceneOverlay.hpp"
#include "WidgetOverlay.hpp"
#include "ProjectLinker.hpp"
#include "CommandPalette.hpp"

#include "AssetSystem.hpp"
#include "RenderSystem.hpp"
#include "EditorSystem.hpp"
#include "WorldSystem.hpp"

#include "EnviromentVariables.hpp"

#include <iostream>
#include <string>
using namespace std;

Engine::Engine() {
	projectLinker = new ProjectLinker;
	
	systemController = new SystemController;
	renderSystem = systemController->addSystem<RenderSystem>();
	assetSystem = systemController->addSystem<AssetsSystem>();
	inputSystem = systemController->addSystem<InputSystem>();
	editorSystem = systemController->addSystem<EditorSystem>();
	worldSystem = systemController->addSystem<WorldSystem>();
}
Engine::~Engine() {
}
void Engine::args(int argc, char *argv[]) {
	auto e = Enviroment::setInstance(new Enviroment);
	e->applicationPath = argv[0];
	e->applicationDir = e->applicationPath.substr(0, e->applicationPath.find_last_of("\\")) + "\\";
	e->resourcePath = e->applicationDir + "\\res\\";
	e->contentPath = e->applicationDir + "\\content\\";
}
void Engine::tick() {
	systemController->initialize();
	systemController->lateInitialize();

	while (true) {
		if (projectLinker->hasNewVersion() || editorSystem->command->isReload) {
			systemController->endPlay();
			projectLinker->unload();
			if (projectLinker->load()) {
				systemController->beginPlay();
			}
			
			editorSystem->command->isReload = false;
		}

		timeStats.capture();
		const double &deltaTime = timeStats.getDeltaTime();
		if (editorSystem->command->isPlay) {
			systemController->tick<ESystemTickType::gameplay>(deltaTime);
			renderSystem->mainWindow.setTitle(to_string(timeStats.getFPS()));
		}
		else {
			systemController->tick<ESystemTickType::editor>(deltaTime);
		}
	}
	systemController->endPlay();
	systemController->shutdown();
}

//systemSeer->initialize();
//systemSeer->lateInitialize();

//auto widget = dynamic_cast<WidgetOverlay*>(renderSystem->getLayer("widget"));
//auto scene = dynamic_cast<SceneOverlay *>(renderSystem->getLayer("scene"));

///// crappy way of controlling the play and stop button
//bool isPlaying = false;
//UI::Canvas win;
//UI::Text txt;
//UI::Button bt;
//bt.text = ">";
//(*widget) << &win;
//win << &txt << &bt;	
////////////////////////////////////////////////////////////////////

//while (true) {
//	hotReload();
//	
//	timeStats.capture();
//	const double& deltaTime = timeStats.getDeltaTime();
//	
//	if (isPlaying) {
//		systemSeer->tick<ESystemTickType::gameplay>(deltaTime);
//		renderSystem->mainWindow.setTitle(to_string(timeStats.getFPS()));
//	}
//	else 
//		systemSeer->tick<ESystemTickType::editor>(deltaTime);


//	if (bt.isClicked()) {
//		if (isPlaying) {
//			isPlaying = false;
//			systemSeer->endPlay();
//			bt.text = ">";
//		}
//		else {
//			isPlaying = true;
//			systemSeer->beginPlay();
//			bt.text = "||";
//		}
//	}
//}
//systemSeer->endPlay();
//systemSeer->shutdown();