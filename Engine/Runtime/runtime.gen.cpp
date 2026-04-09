#include "Runtime.hpp"
#include <Gumball/Engine.hpp>
#include <Input/Input.module.hpp>
#include <Asset/Asset.module.hpp>
#include <Render/Render.module.hpp>
#include <World/World.module.hpp>
#include <Editor/Editor.module.hpp>
void injectModules(Plugin::Controller *mCtrl) {
	mCtrl->AddModule<InputModule>();
	mCtrl->AddModule<AssetModule>();
	mCtrl->AddModule<RenderModule>();
	mCtrl->AddModule<WorldModule>();
	mCtrl->AddModule<EditorModule>();
}
const char* engineDir() {
	return "C:\\Users\\josia\\source\\repos\\JosiasWerly\\Gumball\\";
}
