#pragma once
#ifndef _module
#define _module

#include <list>
#include <unordered_map>

#include "Engine.hpp"

enum class EModuleTickType {
	none,
	editor,
	gameplay,
	all,
};

class GENGINE Module {
private:
	double msCost = 0.0;

protected:
	friend class ModuleController;
	friend class Engine;

	Module() = default;
	virtual ~Module() = default;

	virtual bool Load();
	virtual void Unload();

	virtual void BeginPlay();
	virtual void EndPlay();
	virtual void Tick(const double &deltaTime);

	virtual EModuleTickType TickType() const;

public:
	virtual const char* Name() const;
	double getMsCost() const { return msCost; }
};

class GENGINE ModuleController {
	friend class Engine;

private:
	Codex codex;
	std::list<Module *> modules;
	std::list<Module *> editorTick, gameplayTick;
	std::list<Module *> loadingJobs;

	ModuleController();
	void _addModule(Module *module);
	void Callback_LoadCompleted(Concurrent::Job *job);

protected:
	void Startup();
	void Shutdown();

	void BeginPlay();
	void EndPlay();
	void Tick();
	template<EModuleTickType> void Tick(const double &deltaTime) = delete;
	template<> void Tick<EModuleTickType::editor>(const double &deltaTime);
	template<> void Tick<EModuleTickType::gameplay>(const double &deltaTime);
	bool foo();

public:
	template<class T> void AddModule() {
		T *newModule = new T;
		codex.add<T>(newModule);
		_addModule(newModule);
	}
	template<class T> T *ModuleAt() { return codex.get<T>(); }
	std::list<Module *> &Modules() { return modules; }
};

template<class T> class ModuleSingleton : public Module {
protected:
	ModuleSingleton() = default;
	virtual ~ModuleSingleton() = default;

public:
	static T *instance() {
		static T *inst = Engine::instance()->getModuleController()->ModuleAt<T>();
		return inst;
	}
};

#endif // !_module