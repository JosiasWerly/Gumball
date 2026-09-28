#include "Controller.hpp"
#include <Gumball/Core/Engine.hpp>

using Plugin::Controller;
using Plugin::Module;

//NOTE: i think this has to be fsm, not sure why it is not...
Controller::Controller() {}
void Controller::Startup_OnEnter() {
	state = eState::startup;
	Concurrent::TaskSequence loader;
	for (auto &e : entries) {
		if (Concurrent::Task t = e.module->Load()) {
			*t.Data() = e.module;
			loader.Push(t);
		}
	}
	loader.End().Bind({ this, &Controller::Startup_OnCompleted });
	loader.Start();
}
void Controller::Startup_OnCompleted(const Concurrent::ATask &tsk) {
	if (tsk.IsSuccess()) {
		state = eState::idle;
	}
	else {
		Error("module not loaded {}", tsk.Data().As<Module>()->Name());
	}
}
void Controller::Shutdown_OnEnter() {
	state = eState::shutdown;
	Concurrent::TaskSequence unloader;
	for (auto &e : entries) {
		if (Concurrent::Task t = e.module->Unload()) {
			*t.Data() = e.module;
			unloader.Push(t);
		}
	}
	unloader.End().Bind({ this, &Controller::Startup_OnCompleted });
	unloader.Start();
}
void Controller::Shutdown_OnCompleted(const Concurrent::ATask &tsk) {
	if (tsk.IsSuccess()) {
		state = eState::idle;
	}
	else {
		Error("module not unloaded {}", tsk.Data().As<Module>()->Name());
	}
}
void Controller::Editor_OnEnter() {
}
void Controller::Editor_OnExit() {
}
void Controller::Play_OnEnter() {
}
void Controller::Play_OnExit() {
}
void Controller::HotReload() {
	project.Unload();
	project.Load();
}

void Controller::State(eState st) {
	if (state != st) {
		switch (st) {
			case eState::startup:
				Startup_OnEnter();
				break;
			case eState::shutdown:
				Shutdown_OnEnter();
				break;
			case eState::playing:
				break;
			case eState::hotreload:
				HotReload();
				break;
		}
	}
}

std::list<Module *> Controller::Modules() const {
	std::list<Module *> out;
	for (auto &e : entries)
		out.push_back(e.module);
	return out;
}