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
	fsm[eSignal::play].onEnter.bind([&]() {
		moduleController->beginPlay();
	});
	fsm[eSignal::play].onExit.bind([&]() {
		moduleController->endPlay();
	});
	fsm[eSignal::play].onTick.bind([&]() {
		const double deltaTime = 0.1;
		moduleController->tick<EModuleTickType::gameplay>(deltaTime);
	});

	fsm[eSignal::hotreload].onEnter.bind([&]() {
		moduleController->shutdown();
		projectTarget->unload();
		projectTarget->load();
		moduleController->startup();
		fsm.to(eSignal::idle);
	});
}
void Engine::tick() {

	const double deltaTime = 0.1;

	moduleController->load();
	fsm.set(eSignal::hotreload);
	fsm.tick();
	while (fsm.now() != eSignal::exit) {
		moduleController->tick<EModuleTickType::editor>(deltaTime);
		fsm.tick();
	}
	moduleController->endPlay();
	moduleController->shutdown();
	moduleController->unload();
}