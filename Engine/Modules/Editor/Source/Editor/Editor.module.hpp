#pragma once
#ifndef __editormodule
#define __editormodule

class GMODULE EditorModule : public ModuleSingleton<EditorModule> {
	class UserWidget *editor;
	
	void posLoad() override;
	EModuleTickType tickType() override { return EModuleTickType::gameplay; }

public:
    EditorModule() = default;
};
#endif //__editormodule