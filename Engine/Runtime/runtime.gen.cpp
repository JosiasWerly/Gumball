#include "Runtime.hpp"
#include <Gumball/Engine.hpp>
#include <Asset/Asset.module.hpp>
#include <Render/Render.module.hpp>
#include <World/World.module.hpp>
#include <Input/Input.module.hpp>
#include <Editor/Editor.module.hpp>
void injectModules(ModuleController *mCtrl) {
	mCtrl->addModule<AssetModule>();
	mCtrl->addModule<RenderModule>();
	mCtrl->addModule<WorldModule>();
	mCtrl->addModule<InputModule>();
	mCtrl->addModule<EditorModule>();
}
const char* engineDir() {
	return "C:\\Users\\josia\\source\\repos\\JosiasWerly\\Gumball\\";
}
