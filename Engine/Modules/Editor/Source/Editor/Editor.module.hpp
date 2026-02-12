#pragma once
#ifndef __editormodule
#define __editormodule
//#include <Render/Widget/Widget.hpp>

class GMODULE EditorModule : public ModuleSingleton<EditorModule> {
	class UserWidget *editor;
	
	void Unload() override;
	void Tick(const double &deltaTime) override;
	EModuleTickType TickType() const override { return EModuleTickType::gameplay; }
	const char* Name() const override { return "Editor"; }

public:
    EditorModule() = default;
};
#endif //__editormodule