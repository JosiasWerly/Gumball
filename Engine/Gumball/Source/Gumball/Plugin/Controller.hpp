#pragma once
#ifndef _plugin_controller_
#define _plugin_controller_

#include <Gumball/Concurrent/Task.hpp>
#include <Gumball/Concurrent/Job.hpp>
#include <Gumball/Containers/Codex.hpp>
#include <Gumball/Flow/StateMachine.hpp>
#include "Module.hpp"
#include "Project.hpp"

#include <list>

namespace Core {
	class Engine;
};

namespace Plugin {

struct Module {
	IModule *ptr;
	Concurrent::Job job;

	Module(IModule *init) : ptr(init) {}
};

class GENGINE Controller {
	friend class ::Core::Engine;

	Containers::Codex codex;
	std::list<Module> modules;
	ProjectLinker project;
	Flow::Fsm fsm;

	Controller();
	template<class T> 
	void AddModule();

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
	
	template<class T> 
	T *ModuleAt() { return codex.Get<T>(); }
	
	std::list<IModule *> Modules() const;
};

template<class T>
inline void Controller::AddModule() {
	IModule *newModule = new T;
	codex.Add<T>(newModule);	
	Module &entry = modules.emplace_back();
	entry.ptr = newModule;
}

};
#endif // !_module