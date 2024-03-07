#include "CommandPalette.hpp"
#include "Engine.hpp"
#include "InputSystem.hpp"
#include "WidgetOverlay.hpp"

ToolbarWidget::ToolbarWidget() {
	engine = Engine::instance();
	(*this) << &reload << &play << &stop;
	reload.text = "reload";
	play.text = "play";
	stop.text = "stop";
}
void ToolbarWidget::render(const double &deltaTime){
	beginDraw();
	reload.render(deltaTime);	ImGui::SameLine();
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
	else if (reload.isClicked()) {
		engine->signalLoad();
	}
}

//class CommandPaletteWidget : public UserWidget {
//public:
//	/*CommandPalette *owner;
//	UI::InputText input;
//	UI::Button play;
//	UI::Button reload;
//	CommandPaletteWidget() {
//		(*this) << &reload << &play << &input;
//		play.text = "play";
//		reload.text = "rld";
//	}
//	void render(const double &deltaTime) override {
//		reload.render(deltaTime);
//		ImGui::SameLine();
//		play.render(deltaTime);
//		input.render(deltaTime);
//
//
//		if (play.isClicked()) {
//			bool &isPlay = owner->isPlay;
//			isPlay = !isPlay;
//			if (isPlay) {
//				play.text = "stop";
//			}
//			else {
//				play.text = "play";
//			}
//		}
//		else if (reload.isClicked()) {
//			bool &isReload = owner->isReload;
//			isReload = true;
//		}
//	}*/
//};
//
//CommandPalette::CommandPalette() {
//	CommandPaletteWidget *widget = new CommandPaletteWidget;
//	widget->owner = this;
//	widget->show();
//}