#include "Controller.hpp"
#include <Gumball/Core/Engine.hpp>

using Plugin::Controller;
using Plugin::Module;

Controller::Controller() {
	fsm[eState::startup].OnEnter.Bind(this, &Controller::Startup_OnEnter);
	fsm[eState::shutdown].OnEnter.Bind(this, &Controller::Shutdown_OnEnter);

	fsm[eState::editor].OnEnter.Bind(this, &Controller::Editor_OnEnter);
	fsm[eState::editor].OnExit.Bind(this, &Controller::Editor_OnExit);
	fsm[eState::playing].OnEnter.Bind(this, &Controller::Play_OnEnter);
	fsm[eState::playing].OnExit.Bind(this, &Controller::Play_OnExit);
}
void Controller::Startup_OnEnter() {
	Concurrent::TaskSequence loader;
	for (auto &m : modules) {
		if (Concurrent::Task t = m.ptr->Load()) {
			*t.Data() = m.ptr;
			loader.Push(t);
		}
	}
	if (loader) {
		loader.End().Bind(this, &Controller::Startup_OnCompleted);
		loader.Start();
	}
}
void Controller::Startup_OnCompleted(const Concurrent::ATask &tsk) {
	Assert(tsk.IsSuccess(), "module not loaded {}", tsk.Data().As<IModule>()->Name());
	fsm.To(eState::editor);
}
void Controller::Shutdown_OnEnter() {
	Concurrent::TaskSequence unloader;
	for (auto &m : modules) {
		if (Concurrent::Task t = m.ptr->Unload()) {
			*t.Data() = m.ptr;
			unloader.Push(t);
		}
	}
	unloader.End().Bind(this, &Controller::Startup_OnCompleted);
	unloader.Start();
}
void Controller::Shutdown_OnCompleted(const Concurrent::ATask &tsk) {
	Assert(tsk.IsSuccess(), "module not unloaded {}", tsk.Data().As<IModule>()->Name());
}
void Controller::Editor_OnEnter() {
	for (auto m : modules) {
		const eTick t = m.ptr->TickType();
		const bool hasTick = t == eTick::all || t == eTick::editor;
		if (!m.job.Began() && hasTick)
			m.job.Start();
	}
}
void Controller::Editor_OnExit() {
	for (auto m : modules) {
		const eTick t = m.ptr->TickType();
		const bool hasTick = t == eTick::all || t == eTick::editor;
		if (m.job.Began() && hasTick)
			m.job.Stop();
	}
}
void Controller::Play_OnEnter() {
	for (auto m : modules) {
		const eTick t = m.ptr->TickType();
		const bool hasTick = t == eTick::all || t == eTick::gameplay;
		if (!m.job.Began() && hasTick)
			m.job.Start();
	}
}
void Controller::Play_OnExit() {
	for (auto m : modules) {
		const eTick t = m.ptr->TickType();
		const bool hasTick = t == eTick::all || t == eTick::gameplay;
		if (m.job.Began() && hasTick)
			m.job.Stop();
	}
}
void Controller::HotReload() {
	project.Unload();
	project.Load();
}
std::list<Plugin::IModule *> Controller::Modules() const {
	std::list<Plugin::IModule *> out;
	for (auto &m : modules)
		out.push_back(m.ptr);
	return out;
}

//std::list<IModule *> Controller::Modules() const {
//	std::list<IModule *> out;
//	for (auto &m: modules)
//		out.push_back(m.ptr);
//	return out;
//}