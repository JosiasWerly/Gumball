#include "EditorSystem.hpp"
#include "CommandPalette.hpp"

void EditorSystem::lateInitialize() {
	toolbar = new ToolbarWidget;
	toolbar->show();
	
	command = new CommandPalette;
	command->show();
}