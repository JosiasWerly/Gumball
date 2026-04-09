#include "Controller.hpp"

using Plugin::Controller;
using Plugin::Module;

Controller::Controller() {}
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
}
void Controller::Callback_LoadCompleted(Concurrent::Job *job) {
	const Module *m = job->Data().As<Module>();
	if (job->HasConcluded()) {
		Engine::instance()->Scheduler().Pop(*job);
	}
}
void Controller::Startup() {
	using namespace Concurrent;
	
	for (auto &m : modules) {
		loadJob.Add(m->Name()).Fn().bind({m, &Module::Load});
	}
	loadJob.End().bind({ this, &Controller::Callback_LoadCompleted });
	Engine::instance()->Scheduler().Add(loadJob);
}
void Controller::Shutdown() {
	for (auto &m : modules)
		m->Unload();
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