#pragma once
#ifndef _editorsystem
#define _editorsystem

#include "SystemController.hpp"

class EditorSystem : 
	public System {
public:
	class CommandPalette *command;
	void lateInitialize() override;
	ESystemTickType tickType() override { return ESystemTickType::none; }
};
#endif // !_editorsystem
