#pragma once
#ifndef _plugin_module
#define _plugin_module

#include <Gumball/Concurrent/Task.hpp>

namespace Plugin {

enum class eTick : char {
	none,
	editor,
	gameplay,
	all,
};

class GENGINE Module {
	friend class Core;
	friend class Controller;
protected:

	Module() = default;

	virtual Concurrent::Task Load() { return Concurrent::Task(); }
	virtual Concurrent::Task Unload() { return Concurrent::Task(); }

	virtual void BeginPlay() {}
	virtual void EndPlay() {}
	virtual void Tick(const double &deltaTime) {}

	virtual eTick TickType() const { return eTick::none; }

public:
	virtual ~Module() = default;
	virtual const char *Name() const = 0;
};

};
#endif // !_plugin_module