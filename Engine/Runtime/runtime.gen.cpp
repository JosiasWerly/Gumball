#include "Runtime.hpp"
#include <Gumball/Engine.hpp>
#include <Asset/Asset.module.hpp>
#include <Render/Render.module.hpp>
#include <Input/Input.module.hpp>
#include <Editor/Editor.module.hpp>
#include <World/World.module.hpp>
void injectModules(ModuleController *mCtrl) {
	mCtrl->addModule<AssetModule>();
	mCtrl->addModule<RenderModule>();
	//mCtrl->addModule<InputModule>();
	//mCtrl->addModule<EditorModule>();
	//mCtrl->addModule<WorldModule>();
}
const char* engineDir() {
	return "C:\\Users\\josia\\source\\repos\\JosiasWerly\\Gumball\\";
}
