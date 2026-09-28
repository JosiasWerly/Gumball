#pragma once
#ifndef _plugin_controller_
#define _plugin_controller_

#include <Gumball/Concurrent/Task.hpp>
#include <Gumball/Concurrent/Job.hpp>
#include <Gumball/Containers/Codex.hpp>
#include "Module.hpp"
#include "Project.hpp"

#include <list>

namespace Engine {
	class Core;
};

namespace Plugin {

struct ModuleEntry {
	Module *module;
	Concurrent::Job job;

	ModuleEntry(Module *m) : module(m) {}
};

class GENGINE Controller {
public:	
	enum class eState : char { startup, shutdown, idle, playing, hotreload };

private:
	friend class ::Engine::Core;

	Containers::Codex codex;
	std::list<ModuleEntry> entries;
	ProjectLinker project;
	eState state;


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
	
	void State(eState st);
	eState State() const { return state; }
	
	template<class T> 
	void AddModule() {
		T *newModule = new T;
		codex.Add<T>(newModule);
		entries.emplace_back().module = newModule;
	}
	template<class T> 
	T *ModuleAt() { return codex.Get<T>(); }
	
	std::list<Module *> Modules() const;
};

};
#endif // !_module