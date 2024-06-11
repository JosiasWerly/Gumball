#include "Runtime.hpp"
#include <Gumball/Engine.hpp>
#include <Content/Content.module.hpp>
#include <Editor/Editor.module.hpp>
#include <Input/Input.module.hpp>
#include <Render/Render.module.hpp>
#include <World/World.module.hpp>
void injectModules(ModuleController *mCtrl) {
	mCtrl->addModule<ContentModule>();
	mCtrl->addModule<RenderModule>();
	mCtrl->addModule<InputModule>();
	mCtrl->addModule<EditorModule>();
	mCtrl->addModule<WorldModule>();
}