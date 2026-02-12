#include "Engine.hpp"

#include "Module.hpp"
#include "ProjectTarget.hpp"
#include "Domain.hpp"

#include <iostream>
#include <string>

using namespace std;

Engine::Engine() {
	moduleController = new ModuleController;
	projectTarget = new ProjectTarget;
}
Engine::~Engine() {
}
void Engine::initialize(EngineInit data) {
	data.fnInjectModules(moduleController);
	Domain *domain = new Domain;
	domain->applicationPath = data.argv[0];
	domain->applicationDir = domain->applicationPath.substr(0, domain->applicationPath.find_last_of("\\")) + "\\";
	domain->engineDir = data.engineDir;
	domain->contentPath = domain->engineDir + "Content\\";
	codex.add<Domain>(domain);

	using Ctrl = Flow::StateMachine::Controller;
	fsm[eState::play].onEnter.bind([&]() {
		moduleController->BeginPlay();
	});
	fsm[eState::play].onExit.bind([&]() {
		moduleController->EndPlay();
	});
	fsm[eState::play].onTick.bind([&]() {
		const double deltaTime = 0.1;
		moduleController->Tick<EModuleTickType::gameplay>(deltaTime);
	});

	fsm[eState::hotreload].onEnter.bind([&]() {
		projectTarget->unload();
		projectTarget->load();
		fsm.to(eState::idle);
	});
}
void Engine::tick() {

	const double deltaTime = 0.1;
	scheduler.Start(0);
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