#pragma once
#ifndef __editormodule
#define __editormodule

class GMODULE EditorModule : public ModuleSingleton<EditorModule> {
	//class UserWidget *editor;
	
	void posLoad() override;
	EModuleTickType tickType() const override { return EModuleTickType::gameplay; }
	const char* name() const override { return "Editor"; }

public:
    EditorModule() = default;
};
#endif //__editormodule