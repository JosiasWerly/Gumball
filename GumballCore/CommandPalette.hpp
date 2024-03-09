#pragma once
#ifndef _consolepalette
#define _consolepalette

#include "WidgetOverlay.hpp"

#include "Event.hpp"
#include <unordered_map>
#include <vector>

using namespace std;

class ToolbarWidget : public UserWidget {
private:
	UI::Button play;
	UI::Button stop;
	UI::Button load;
	class Engine *engine;

	bool isPlay = false;
	bool isLoaded = true;
public:
	ToolbarWidget();
	void render(const double &deltaTime) override;
};


class CommandPalette : public UserWidget {
private:
	UI::InputText input;
	class Engine *engine;
	

	typedef void(*CommandSignature)(const vector<string> &);
	unordered_map<string, CommandSignature> commands;
public:

	CommandPalette();
	void render(const double &deltaTime) override;
};


#endif // !_consolepalette