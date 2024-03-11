#include "Engine.hpp"
#include "SceneOverlay.hpp"
#include "WidgetOverlay.hpp"
#include "ProjectLinker.hpp"
#include "CommandPalette.hpp"

#include "SystemController.hpp"
#include "AssetSystem.hpp"
#include "RenderSystem.hpp"
#include "EditorSystem.hpp"
#include "WorldSystem.hpp"

#include "Activator.hpp"
#include "Package.Gumball.hpp"

#include "EnviromentVariables.hpp"

#include <iostream>
#include <string>
using namespace std;



Engine::Engine() {
	Activator::setInstance(new Activator);
	Activator::instance()->addPackage<ClassTypePackage_Core>();
	test();

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
	
	projectLinker->load();

	while (true) {
		timeStats.capture();
		const double &deltaTime = timeStats.getDeltaTime();

		//cheapest state machine ever created
		switch (loadState) {
			case ELoadState::load:
				projectLinker->load();
				loadState = ELoadState::none;
				break;
			case ELoadState::unload:
				worldSystem->shutdown();//TEMPORARY CALL FOR
				projectLinker->unload();
				loadState = ELoadState::none;
				break;
		}
		
		systemController->tick<ESystemTickType::editor>(deltaTime);

		switch (playState) {
			case EPlayState::beginPlay:
				systemController->beginPlay();
				playState = EPlayState::playing;
				break;
			case EPlayState::playing:
				systemController->tick<ESystemTickType::gameplay>(deltaTime);
				renderSystem->mainWindow.setTitle(to_string(timeStats.getFPS()));
				break;
			case EPlayState::endPlay:
				systemController->endPlay();
				playState = EPlayState::disabled;
				break;
		}
	}
	systemController->endPlay();
	systemController->shutdown();
}