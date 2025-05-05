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

	virtual void load();
	virtual void unload();
	virtual void posLoad();

	virtual void startup();
	virtual void shutdown();
	virtual void hotreload();

	virtual void beginPlay();
	virtual void endPlay();
	virtual void tick(const double &deltaTime);

	virtual EModuleTickType tickType() const;

public:
	virtual const char* name() const;
	double getMsCost() const { return msCost; }
};

class GENGINE ModuleController {
	friend class Engine;

private:
	Codex codex;
	std::list<Module *> modules;
	std::list<Module *> editorTick, gameplayTick;

	ModuleController();
	void _addModule(Module *module);

protected:
	void load();
	void unload();
	void startup();
	void shutdown();
	void beginPlay();
	void endPlay();

	template<EModuleTickType> void tick(const double &deltaTime) = delete;
	template<> void tick<EModuleTickType::editor>(const double &deltaTime);
	template<> void tick<EModuleTickType::gameplay>(const double &deltaTime);

public:
	template<class T> void addModule() {
		T *newModule = new T;
		codex.add<T>(newModule);
		_addModule(newModule);
	}
	template<class T> T *getModule() { return codex.get<T>(); }
	std::list<Module *> &getModules() { return modules; }
};

template<class T> class ModuleSingleton : public Module {
protected:
	ModuleSingleton() = default;
	virtual ~ModuleSingleton() = default;

public:
	static T *instance() {
		static T *inst = Engine::instance()->getModuleController()->getModule<T>();
		return inst;
	}
};

#endif // !_module