#include "Editor.module.hpp"
#include <Render/Render.module.hpp>
#include <Render/Widget/Widget.hpp>
#include <Input/Input.module.hpp>



class EditorToolbar : public UserWidget {
public:
	EditorToolbar() {
		using namespace Glyph;
		label("EditorToolbar");
		visibility(eVisibility::visible);

		glm::fvec3 foo(1, 2, 3);
		Vector3 vi = foo;

		Text *text = new Text;
		text->text("AUSDIUHASDIUHASIUDHIUASHDIUHASDIUHASIUDHIUASHDILUHASDLIUHALIUSHDLIUASHDLIUHASLDIUHLIUASHDLIUHASDLIUHASLIUDHLIUASHDILUHASILUDH");
		
		
		TextInput *textInA = new TextInput;
		textInA->text("asdf");
		textInA->multiline(true);
		
		TextInput *textInB = new TextInput;
		textInB->text("FOO_BAR");
		textInB->multiline(false);
		
		ColorPicker *color = new ColorPicker;
		
		Combo *combo = new Combo;
		combo->setItems(std::vector<string>{"a", "B", "c"});
		combo->label("combox");
		
		Container *horizontalBox = new Container;
		horizontalBox->label("BunchOfButtons");
		horizontalBox->layout(eLayout::horizontal);
		for (size_t i = 0; i < 4; i++)
			horizontalBox->container << new Button;
		
		
		container << Widgets{
			horizontalBox, text, textInA, textInB, combo, color, new IntergerInput(1), new FloatInput(2),
			new ProgressBar,
			new Histogram
		};

		Text *tMem = new Text;
		tMem->text(std::to_string(RenderModule::instance()->getWidgetOverlay().memory));
		container << tMem;
	}
};




void EditorModule::posLoad() {
	new EditorToolbar;
}

//using namespace Clay;
//class EditorToolbar : public UserWidget {
//private:
//	Button play, stop;
//	ProgressBar prog;
//	Histogram hist;
//	float a = 0;
//
//public:
//	EditorToolbar() {
//		(*this) << Widgets{&play, &stop, &prog, &hist,};
//
//		setLabel("toolbar");
//		play.setLabel("pause");
//		stop.setLabel("stop");
//		hist.setSize({ 400, 80 });
//
//
//		play.onClick.bindFunction(
//			[](Widget *obj, bool newState) {
//				if (newState) {
//					static_cast<Button &>(*obj).setLabel("pause");
//					Engine::instance()->getState().newSignal(Engine::State::ESignal::play);
//				}
//				else {
//					static_cast<Button &>(*obj).setLabel("play");
//					Engine::instance()->getState().newSignal(Engine::State::ESignal::pause);
//				}
//			}
//		);
//
//		stop.onClick.bindFunction(
//			[](Widget *obj, bool newState) {
//				Engine::instance()->getState().newSignal(Engine::State::ESignal::stop);
//			}
//		);
//	}
//	void render(const double &deltaTime) {
//		a += 0.1;
//
//		UserWidget::render(deltaTime);
//		if (prog.getValue() >= 100) {
//			prog.setValue(0.f);
//		}
//		else {
//			prog.setValue(prog.getValue() + 0.1);
//		}
//		hist.pushValue(sin(a));
//	}
//};
//
//class ModuleControllerDebug : public UserWidget {
//private:
//	vector<Histogram> histogram;
//
//public:
//	ModuleControllerDebug() {
//		setLabel("ModuleControllerDebug");
//		std::list<Module *> &modules = Engine::instance()->getModuleController()->getModules();
//		histogram.resize(modules.size());
//		
//		int i = 0;
//		for (auto m : modules) {
//			histogram[i].setLabel(m->name());
//			(*this)<<&histogram[i];
//			i++;
//		}
//	}
//	void render(const double &deltaTime) {
//		std::list<Module *> &modules = Engine::instance()->getModuleController()->getModules();
//		histogram.resize(modules.size());
//
//		int i = 0;
//		for (auto m:modules) {
//			histogram[i++].pushValue(m->getMsCost());
//		}
//	}
//};
//
//void EditorModule::posLoad() {
//	editor = new EditorToolbar;
//	editor->show();
//
//	ModuleControllerDebug *debug = new ModuleControllerDebug();
//	debug->show();
//}