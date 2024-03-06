#include "EditorSystem.hpp"
#include "CommandPalette.hpp"

void EditorSystem::lateInitialize() {
	command = new CommandPalette;
	//widget->owner = this;
	//widget->show();
}