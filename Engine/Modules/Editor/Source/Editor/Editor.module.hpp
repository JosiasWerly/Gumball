#pragma once
#ifndef __editormodule
#define __editormodule
//#include <Render/Widget/Widget.hpp>

class GMODULE EditorModule : public ModuleSingleton<EditorModule> {
	class UserWidget *editor;
	
	void posLoad() override;
	void tick(const double &deltaTime) override;
	EModuleTickType tickType() const override { return EModuleTickType::gameplay; }
	const char* name() const override { return "Editor"; }

public:
    EditorModule() = default;
};
#endif //__editormodule