#pragma once
#ifndef __editormodule
#define __editormodule
//#include <Render/Widget/Widget.hpp>

class GMODULE EditorModule : public Plugin::Singleton<EditorModule> {
	class UserWidget *editor;
	
	void Unload() override;
	void Tick(const double &deltaTime) override;
	Plugin::eTick TickType() const override { return Plugin::eTick::gameplay; }
	const char* Name() const override { return "Editor"; }

public:
    EditorModule() = default;
};
#endif //__editormodule