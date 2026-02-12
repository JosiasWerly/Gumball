#include "Module.hpp"
#include "Timer.hpp"

bool Module::Load() { return true; }
void Module::Unload() {}
void Module::BeginPlay() {}
void Module::EndPlay() {}
void Module::Tick(const double &deltaTime) {}
EModuleTickType Module::TickType() const { return EModuleTickType::none; }
const char* Module::Name() const { return "Module"; }

ModuleController::ModuleController() {
}
void ModuleController::_addModule(Module *newModule) {
	using namespace Concurrent;

	modules.push_back(newModule);
	EModuleTickType tick = newModule->TickType();
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

	Concurrent::Scheduler &sc = Engine::instance()->Scheduler();
	Job *newJob = sc.Add();
	newJob->data = newModule;
	
	newJob->Add()->fn.bind({ newModule, &Module::Load });
	newJob->end.bind({ this, &ModuleController::Callback_LoadCompleted });
	loadingJobs.push_back(newModule);
}
void ModuleController::Callback_LoadCompleted(Concurrent::Job *job) {
	Module *m = static_cast<Module *>(job->data);
	loadingJobs.remove(m);
	Engine::instance()->Scheduler().Pop(job);
}
void ModuleController::Startup() {
	//for (auto &m : modules) {
	//	newJob->Add()->fn.bind({ m, &Module::Load });
	//	loadingJobs.push_back(newJob);
	//}
}
void ModuleController::Shutdown() {
	//for (auto &m : modules)
	//	m->Unload();
}
void ModuleController::BeginPlay() {
	for (auto &s : modules)
		s->BeginPlay();
}
void ModuleController::EndPlay() {
	for (auto &s : modules)
		s->EndPlay();
}
template<> void ModuleController::Tick<EModuleTickType::editor>(const double& deltaTime) {
	Timer t;
	for (auto &m : editorTick) {
		t.start();
		m->Tick(deltaTime);
		m->msCost = t.milliseconds();
	}
}
template<> void ModuleController::Tick<EModuleTickType::gameplay>(const double& deltaTime) {
	Timer t;
	for (auto &m : gameplayTick) {
		t.start();
		m->Tick(deltaTime);
		m->msCost = t.milliseconds();
	}
}