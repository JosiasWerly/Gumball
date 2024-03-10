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


#include "Actor.hpp"
void SpawnCommand(const vector<string> &args) {
	//TypeWrapper *type = getTypeString(args[0].c_str());
	//if (!type) {
	//	return;
	//}
	//if (Actor *actor = static_cast<Actor *>(type->getNew())) {
	//	actor->transform.position.x = std::stof(args[1]);
	//	actor->transform.position.y = std::stof(args[2]);
	//	actor->transform.position.z = std::stof(args[3]);
	//}
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
		//spawn Actor 2 3 4
		{
			size_t j = 0, i = 0;
			while(true) {
				if (in[i] == ' ' && j < i) {
					arguments.push_back(in.substr(j, (i - j)));
					j = i + 1;
				}
				
				if (!(++i < in.size())) {
					if (j < i) {
						arguments.push_back(in.substr(j, (i - j)));
					}
					break;
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