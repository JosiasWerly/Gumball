#include "EngineSystem.hpp"


IEngineSystem::IEngineSystem() {}
IEngineSystem::~IEngineSystem() {}
void IEngineSystem::initialize() {}
void IEngineSystem::shutdown() {}
void IEngineSystem::onPlay() {}
void IEngineSystem::onEndplay() {}
void IEngineSystem::tick(const double &deltaTime) {}