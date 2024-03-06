#pragma once
#ifndef _editorsystem
#define _editorsystem

#include "ProjectLinker.hpp"
#include "Subsystem.hpp"

class EditorSystem : public EngineSystem {
public:
	class CommandPalette *command;
	void lateInitialize() override;
	ESystemTickType tickType() override { return ESystemTickType::disable; }
};
#endif // !_editorsystem
