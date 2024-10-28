#include "Module.hpp"
#include "Timer.hpp"

void Module::load() {}
void Module::posLoad() {}
void Module::unload() {}
void Module::startup() {}
void Module::shutdown() {}
void Module::hotreload() {}
void Module::beginPlay() {}
void Module::endPlay() {}
void Module::tick(const double &deltaTime) {}
EModuleTickType Module::tickType() const { return EModuleTickType::none; }
const char* Module::name() const { return "Module"; }

ModuleController::ModuleController() {
}
void ModuleController::_addModule(Module *newModule) {
	modules.push_back(newModule);
	EModuleTickType tick = newModule->tickType();
	switch (tick) {
		case EModuleTickType::editor:
			editorTick.push_back(newModule);
			break;
		case EModuleTickType::gameplay:
			gameplayTick.push_back(newModule);
			break;
		case EModuleTickType::all:
			editorTick.push_back(newModule);
			gameplayTick.push_back(newModule);
			break;
	}
}
void ModuleController::load() {
	for (auto &m : modules)
		m->load();
	for (auto &m : modules)
		m->posLoad();
}
void ModuleController::unload() {
	for (auto &m : modules)
		m->unload();
}
void ModuleController::startup() {
	for (auto &m : modules)
		m->startup();
}
void ModuleController::shutdown() {
	for (auto &m : modules)
		m->shutdown();
}
void ModuleController::beginPlay() {
	for (auto &s : modules)
		s->beginPlay();
}
void ModuleController::endPlay() {
	for (auto &s : modules)
		s->endPlay();
}
template<> void ModuleController::tick<EModuleTickType::editor>(const double& deltaTime) {
	Timer t;
	for (auto &m : editorTick) {
		t.start();
		m->tick(deltaTime);
		m->msCost = t.getMilliseconds();
	}
}
template<> void ModuleController::tick<EModuleTickType::gameplay>(const double& deltaTime) {
	Timer t;
	for (auto &m : gameplayTick) {
		t.start();
		m->tick(deltaTime);
		m->msCost = t.getMilliseconds();		
	}
}