#pragma once
#ifndef _plugin_module
#define _plugin_module

#include <list>
#include <unordered_map>

#include "Engine.hpp"

namespace Plugin {
class Controller;

enum class eTick {
	none,
	editor,
	gameplay,
	all,
};

class GENGINE Module {
private:
	double msCost = 0.0;

protected:
	friend class Controller;
	friend class Engine;

	Module() = default;
	virtual ~Module() = default;

	virtual bool Load() { return true; }
	virtual void Unload() {}

	virtual void BeginPlay() {}
	virtual void EndPlay() {}
	virtual void Tick(const double &deltaTime) {}

	virtual eTick TickType() const { return eTick::none; }

public:
	virtual const char *Name() const = 0;
	double getMsCost() const { return msCost; }
};

};
#endif // !_plugin_module