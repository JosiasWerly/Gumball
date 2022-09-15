#include "EngineSystem.hpp"

IEngineSystem::IEngineSystem() {}
IEngineSystem::~IEngineSystem() {}
void IEngineSystem::initialize() {}
void IEngineSystem::shutdown() {}
void IEngineSystem::tick(float deltaTime) {}
void IEngineSystem::onPlay() {}
void IEngineSystem::onEndplay() {}