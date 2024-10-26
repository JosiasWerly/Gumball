#include "Runtime.hpp"
#include <Gumball/Engine.hpp>
#include <Content/Content.module.hpp>
#include <Render/Render.module.hpp>
#include <Input/Input.module.hpp>
#include <World/World.module.hpp>
#include <Editor/Editor.module.hpp>
void injectModules(ModuleController *mCtrl) {
	mCtrl->addModule<ContentModule>();
	mCtrl->addModule<RenderModule>();
	mCtrl->addModule<InputModule>();
	mCtrl->addModule<WorldModule>();
	mCtrl->addModule<EditorModule>();
}
const char* engineDir() {
	return "C:\\Users\\josia\\source\\repos\\JosiasWerly\\Gumball\\";
}
