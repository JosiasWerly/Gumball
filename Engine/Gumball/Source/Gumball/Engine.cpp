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
}
void Engine::tick() {

	const double deltaTime = 0.1;
	using Ctrl = Flow::StateMachine::Controller;
	fsm[eSignal::play].onEnter.bind([&](Ctrl &) {
		moduleController->beginPlay();
	});
	fsm[eSignal::play].onExit.bind([&](const Ctrl &) {
		moduleController->endPlay();
	});
	fsm[eSignal::play].onTick.bind([&](Ctrl &) {
		const double deltaTime = 0.1;
		moduleController->tick<EModuleTickType::gameplay>(deltaTime);
	});

	fsm[eSignal::hotreload].onEnter.bind([&](Ctrl &c) {
		moduleController->shutdown();
		projectTarget->unload();
		projectTarget->load();
		moduleController->startup();

		c.to(eSignal::idle);
	});
	fsm.set(eSignal::hotreload);

	moduleController->load();
	while (fsm.hash() != eSignal::exit) {
		moduleController->tick<EModuleTickType::editor>(deltaTime);
		fsm.run();
		fsm.tick();
	}
	moduleController->endPlay();
	moduleController->shutdown();
	moduleController->unload();
}