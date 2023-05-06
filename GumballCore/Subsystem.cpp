#include "Subsystem.hpp"


EngineSystem::EngineSystem() {}
EngineSystem::~EngineSystem() {}
void EngineSystem::initialize() {}
void EngineSystem::lateInitialize() {}
void EngineSystem::shutdown() {}
void EngineSystem::beginPlay() {}
void EngineSystem::endPlay() {}
void EngineSystem::tick(const double &deltaTime) {}
ESystemTickType EngineSystem::tickType() { return ESystemTickType::disable; }

SystemOverseer::SystemOverseer() {
}
Inline void SystemOverseer::initialize() const {
	for (auto &s : systems)
		s->initialize();
}
Inline void SystemOverseer::lateInitialize() const {
	for (auto &s : systems)
		s->lateInitialize();
}
Inline void SystemOverseer::shutdown() const {
	for (auto &s : systems)
		s->shutdown();
}
Inline void SystemOverseer::beginPlay() const {
	for (auto &s : systems)
		s->beginPlay();
}
Inline void SystemOverseer::endPlay() const {
	for (auto &s : systems)
		s->endPlay();
}