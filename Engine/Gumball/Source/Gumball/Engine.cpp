#include "Engine.hpp"

#include "Plugin/Controller.hpp"
#include "ProjectTarget.hpp"
#include "Domain.hpp"

#include <iostream>
#include <string>

using namespace std;

Engine::Engine() {
	pluginCtrl = new Plugin::Controller;
	projectTarget = new ProjectTarget;
}
Engine::~Engine() {
}
void Engine::initialize(EngineInit data) {
	scheduler = data.scheduler;
	data.fnInjectModules(pluginCtrl);
	Domain *domain = new Domain;
	domain->applicationPath = data.argv[0];
	domain->applicationDir = domain->applicationPath.substr(0, domain->applicationPath.find_last_of("\\")) + "\\";
	domain->engineDir = data.engineDir;
	domain->contentPath = domain->engineDir + "Content\\";
	codex.Add<Domain>(domain);

	fsm[eState::play].onEnter.bind([&]() {
		pluginCtrl->BeginPlay();
	});
	fsm[eState::play].onExit.bind([&]() {
		pluginCtrl->EndPlay();
	});
	fsm[eState::play].onTick.bind([&]() {
		const double deltaTime = 0.1;
		pluginCtrl->Tick<Plugin::eTick::gameplay>(deltaTime);
	});
	fsm[eState::hotreload].onEnter.bind([&]() {
		pluginCtrl->BeginHotReload();
		projectTarget->unload();
		projectTarget->load();
		pluginCtrl->EndHotReload();
		fsm.to(eState::idle);
	});
	
	pluginCtrl->Startup();
}
void Engine::tick() {
	while (true);
	//moduleController->Startup();
	//fsm.set(eState::hotreload);
	//fsm.tick();
	//scheduler.Start(1);
	//while (fsm.now() != eState::exit) {
	//	moduleController->Tick<EModuleTickType::editor>(deltaTime);
	//	fsm.tick();
	//}
	//moduleController->Shutdown();
}