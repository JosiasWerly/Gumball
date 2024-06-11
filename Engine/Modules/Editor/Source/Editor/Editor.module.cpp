#include "Editor.module.hpp"
#include <Render/Render.module.hpp>
#include <Render/Widget/WidgetOverlay.hpp>
#include <Input/Input.module.hpp>

class EditorToolbar : public UserWidget {
private:
	UI::Button play;
	UI::Button stop;
	class Engine *engine;

	bool isPlay = false;
	bool isLoaded = true;
public:
	EditorToolbar() {
		setName("toolbar");
		engine = Engine::instance();
		(*this) << &play << &stop;
		play.text = "play";
		stop.text = "stop";
	}
	void render(const double &deltaTime) override {
		beginDraw();
		stop.render(deltaTime);	//ImGui::SameLine();
		play.render(deltaTime);
		endDraw();


		if (play.isClicked()) {
			isPlay = !isPlay;
			if (isPlay) {
				play.text = "pause";
				engine->getState().newSignal(Engine::State::ESignal::play);
			}
			else {
				play.text = "play";
				engine->getState().newSignal(Engine::State::ESignal::pause);
			}
		}
		else if (stop.isClicked()) {
			engine->getState().newSignal(Engine::State::ESignal::stop);
		}
		//else if (load.isClicked()) {
		//	isLoaded = !isLoaded;
		//	if (isLoaded) {
		//		load.text = "unload";
		//		//engine->signalLoad();
		//	}
		//	else {
		//		load.text = "load";
		//		//engine->signalUnload();
		//	}
		//}
	}
};

void EditorModule::posLoad() {
	editor = new EditorToolbar;
	editor->show();
}