#include "Controller.hpp"

using Plugin::Controller;
using Plugin::Module;

Controller::Controller() {
}
void Controller::AddModule(Module *newModule) {
	modules.push_back(newModule);
	eTick tick = newModule->TickType();
	switch (tick) {
		case eTick::editor:
			editorTick.push_back(newModule);
			break;
		case eTick::gameplay:
			gameplayTick.push_back(newModule);
			break;
		case eTick::all:
			editorTick.push_back(newModule);
			gameplayTick.push_back(newModule);
			break;
	}

	//Concurrent::Scheduler &sc = Engine::instance()->Scheduler();
	//Job *newJob = sc.Add();
	//newJob->data = newModule;
	//newJob->Add()->fn.bind({ newModule, &Module::Load });
	//newJob->end.bind({ this, &ModuleController::Callback_LoadCompleted });
	loadingJobs.push_back(newModule);
}
void Controller::Callback_LoadCompleted(Concurrent::Job *job) {
	Module *m = static_cast<Module *>(job->data);
	loadingJobs.remove(m);
	Engine::instance()->Scheduler().Pop(job);
}
void Controller::Startup() {
	using namespace Concurrent;
	Concurrent::Scheduler &sc = Engine::instance()->Scheduler();
	for (auto &m : modules) {
		Concurrent::Scheduler &sc = Engine::instance()->Scheduler();
		Job *newJob = sc.Add();
		newJob->data = m;
		newJob->Add()->fn.bind({ m, &Module::Load });
		newJob->end.bind({ this, &Controller::Callback_LoadCompleted });
	}
}
void Controller::Shutdown() {
	//for (auto &m : modules)
	//	m->Unload();
}

void Controller::BeginHotReload() {
}
void Controller::EndHotReload() {
}
void Controller::BeginPlay() {
	for (auto &s : modules)
		s->BeginPlay();
}
void Controller::EndPlay() {
	for (auto &s : modules)
		s->EndPlay();
}
template<> void Controller::Tick<Plugin::eTick::editor>(const double& deltaTime) {
	for (auto &m : editorTick) {
		m->Tick(deltaTime);
	}
}
template<> void Controller::Tick<Plugin::eTick::gameplay>(const double& deltaTime) {
	for (auto &m : gameplayTick) {
		m->Tick(deltaTime);
	}
}