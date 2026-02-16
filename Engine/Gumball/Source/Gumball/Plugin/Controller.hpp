#pragma once
#ifndef _plugin_controller_
#define _plugin_controller_

#include <list>
#include <unordered_map>

#include "Engine.hpp"
#include "Module.hpp"

namespace Plugin {
	//class Module;

class GENGINE Controller {
	friend class Engine;

private:
	Codex codex;
	std::list<Module *> modules;
	std::list<Module *> editorTick, gameplayTick;
	std::list<Module *> loadingJobs;

	Controller();
	void AddModule(Module *module);
	void Callback_LoadCompleted(Concurrent::Job *job);

protected:
	void Startup();
	void Shutdown();
	void BeginHotReload();
	void EndHotReload();
	void BeginPlay();
	void EndPlay();

	template<Plugin::eTick> void Tick(const double &deltaTime) = delete;
	template<> void Tick<eTick::editor>(const double &deltaTime);
	template<> void Tick<eTick::gameplay>(const double &deltaTime);

public:
	template<class T> void AddModule() {
		T *newModule = new T;
		codex.Add<T>(newModule);
		AddModule(newModule);
	}
	template<class T> T *ModuleAt() { return codex.Get<T>(); }
	std::list<Module *> &Modules() { return modules; }
	bool IsModuleLoading(const Module &trg) const { return std::find(loadingJobs.begin(), loadingJobs.end(), &trg) != loadingJobs.end(); }
};

template<class T> class Singleton : public Module {
protected:
	Singleton() = default;
	virtual ~Singleton() = default;

public:
	static T *instance() {
		static T *inst = Engine::instance()->PluginController().ModuleAt<T>();
		return inst;
	}
};
};
#endif // !_module