#include "CommandPalette.hpp"
#include "Engine.hpp"
#include "InputSystem.hpp"
#include "WidgetOverlay.hpp"

class ConsoleWidget : public UserWidget {
public:
	CommandPalette *palette;
	UI::InputText input;
	UI::Button play;
	UI::Button reload;
	ConsoleWidget() {
		(*this) << &reload << &play << &input;
		play.text = "play";
		reload.text = "rld";
	}
	void render(const double &deltaTime) override {
		reload.render(deltaTime);
		ImGui::SameLine();
		play.render(deltaTime);
		input.render(deltaTime);


		if (play.isClicked()) {
			bool &isPlay = palette->isPlay;
			isPlay = !isPlay;
			if (isPlay) {
				play.text = "stop";
			}
			else {
				play.text = "play";
			}
		}
		else if (reload.isClicked()) {
			bool &isReload = palette->isReload;
			isReload = true;
		}
	}
};

CommandPalette::CommandPalette() {
	ConsoleWidget *widget = new ConsoleWidget;
	widget->palette = this;
	widget->show();
}