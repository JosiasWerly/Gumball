#pragma once
#ifndef __plugincontroller
#define __plugincontroller

#include <Gumball/Concurrent/Job.hpp>
#include <Gumball/Flow/StateMachine.hpp>
#include <Gumball/Core/Engine.hpp>
#include "Module.hpp"
#include "Project.hpp"
#include <list>

namespace Plugin {

struct Module {
	IModule *ptr;
	Concurrent::Job job;

	Module(IModule *init) : ptr(init) {}
};

class GENGINE Controller : public ::Core::Global<Controller> {
	friend class ::Core::Engine;

	std::list<Module> modules;
	ProjectLinker project;
	Flow::Fsm fsm;

	Controller();

	void Startup_OnEnter();
	void Startup_OnCompleted(const Concurrent::ATask &tsk);
	void Shutdown_OnEnter();
	void Shutdown_OnCompleted(const Concurrent::ATask &tsk);
	void Editor_OnEnter();
	void Editor_OnExit();
	void Play_OnEnter();
	void Play_OnExit();
	void HotReload();

public:

	enum class eState : char { startup, shutdown, editor, playing, hotreload };
	void State(eState st) { fsm.To(st); fsm.Tick(); }
	eState State() const { return fsm.Now(); }
	
	template<class T> void Add();
	std::list<IModule *> Modules() const;
};

template<class T>
inline void Controller::Add() {
	IModule *newModule = (IModule *) & ::Core::Engine::Instance().Codex().Add<T>();
	Plugin::Module &entry = modules.emplace_back(newModule);
}

};
#endif // !__plugincontroller