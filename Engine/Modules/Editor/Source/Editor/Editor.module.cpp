#include "Editor.module.hpp"
#include <Render/Render.module.hpp>
#include <Render/Widget/WidgetOverlay.hpp>
#include <Input/Input.module.hpp>

using namespace Clay;
class EditorToolbar : public UserWidget {
private:
	Button play, stop;

public:
	EditorToolbar() {
		layout = ELayout::horizontal;
		
		setLabel("toolbar");
		pushItems({ &play, &stop });
		play.setLabel("pause");
		stop.setLabel("stop");


		play.onClick.bindFunction(
			[](Widget *obj, bool newState) {
				if (newState) {
					static_cast<Button &>(*obj).setLabel("pause");
					Engine::instance()->getState().newSignal(Engine::State::ESignal::play);
				}
				else {
					static_cast<Button &>(*obj).setLabel("play");
					Engine::instance()->getState().newSignal(Engine::State::ESignal::pause);
				}
			}
		);

		stop.onClick.bindFunction(
			[](Widget *obj, bool newState) {
				Engine::instance()->getState().newSignal(Engine::State::ESignal::stop);
			}
		);
	}
};

void EditorModule::posLoad() {
	editor = new EditorToolbar;
	editor->show();
}