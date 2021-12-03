#pragma once
#ifndef __assetManager
#define __assetManager

#include "EngineSystem.hpp"

class AssetsSystem :
	public IEngineSystem {
public:
	virtual void Initialize() {}
	virtual void Shutdown() {}
};

#endif