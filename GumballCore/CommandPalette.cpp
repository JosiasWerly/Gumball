#include "CommandPalette.hpp"
#include "Engine.hpp"
#include "InputSystem.hpp"
#include "WidgetOverlay.hpp"

ToolbarWidget::ToolbarWidget() {
	setName("toolbar");
	engine = Engine::instance();
	(*this) << &load << &play << &stop;
	load.text = "unload";
	play.text = "play";
	stop.text = "stop";
}
void ToolbarWidget::render(const double &deltaTime){
	beginDraw();
	load.render(deltaTime);	ImGui::SameLine();
	stop.render(deltaTime);		ImGui::SameLine();
	play.render(deltaTime);
	endDraw();
	
	
	if (play.isClicked()) {
		isPlay = !isPlay;
		if (isPlay) {
			play.text = "pause";
			engine->signalPlay();
		}
		else {
			play.text = "play";
			engine->signalPause();
		}
	}
	else if (stop.isClicked()) {
		engine->signalStop();
	}
	else if (load.isClicked()) {
		isLoaded = !isLoaded;
		if (isLoaded) {
			load.text = "unload";
			engine->signalLoad();
		}
		else {
			load.text = "load";
			engine->signalUnload();
		}
	}
}



void SpawnCommand(const vector<string> &Args) {

}

CommandPalette::CommandPalette() {
	setName("command");
	engine = Engine::instance();
	(*this) << &input;

	commands["spawn"] = SpawnCommand;
}
void CommandPalette::render(const double &deltaTime) {
	beginDraw();
	input.render(deltaTime);
	endDraw();

	string in;
	if (input.getInput(in)) {
		vector<string> arguments;

		{
			size_t j = 0;
			for (size_t i = 0; i < in.size(); ++i) {
				if (in[i] == ' ' && j < i) {
					arguments.push_back(in.substr(j, (i - j)));
				}
			}
		}

		if (!arguments.size()) {
			return;
		}
		
		string command = arguments[0];
		arguments.erase(arguments.begin());
		if (commands.contains(command)) {
			commands[command](arguments);
		}
	}
}