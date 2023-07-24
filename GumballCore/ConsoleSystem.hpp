#pragma once
#ifndef _consolesystem
#define _consolesystem

#include "GLUtils.hpp"
#include "ProjectLinker.hpp"
#include "Subsystem.hpp"

//Vis consolari.
class ConsoleSystem : public EngineSystem {
public:
	bool isPlay = false;
	bool isReload = false;

	void lateInitialize() override;
	void tick(const double &deltaTime) override;
	ESystemTickType tickType() override { return ESystemTickType::disable; }
};
#endif // !_consolesystem
