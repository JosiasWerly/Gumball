#pragma once
#ifndef _consolepalette
#define _consolepalette

#include "WidgetOverlay.hpp"

class ToolbarWidget : public UserWidget {
private:
	UI::Button play;
	UI::Button stop;
	UI::Button reload;
	Engine *engine;

	bool isPlay = false;
public:
	ToolbarWidget();
	void render(const double &deltaTime) override;
};


#endif // !_consolepalette