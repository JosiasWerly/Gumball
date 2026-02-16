#include "Runtime.hpp"
#include <Gumball/Engine.hpp>
#include <Render/Render.module.hpp>
#include <Input/Input.module.hpp>
#include <World/World.module.hpp>
#include <Editor/Editor.module.hpp>
#include <Asset/Asset.module.hpp>
void injectModules(Plugin::Controller *mCtrl) {
	mCtrl->AddModule<RenderModule>();
	mCtrl->AddModule<InputModule>();
	mCtrl->AddModule<WorldModule>();
	mCtrl->AddModule<EditorModule>();
	mCtrl->AddModule<AssetModule>();
}
const char* engineDir() {
	return "C:\\Users\\josia\\source\\repos\\JosiasWerly\\Gumball\\";
}
