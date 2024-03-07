#include "EditorSystem.hpp"
#include "CommandPalette.hpp"

void EditorSystem::lateInitialize() {
	toolbar = new ToolbarWidget;
	toolbar->setName("toolbar");
	toolbar->show();
}