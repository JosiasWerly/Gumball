#include "ConsoleSystem.hpp"
#include "ConsoleSystem.hpp"
#include "Engine.hpp"
#include "InputSystem.hpp"
#include "WidgetOverlay.hpp"

class ConsoleWidget : public UserWidget {
public:
	ConsoleSystem *owner;
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
			bool &isPlay = owner->isPlay;
			isPlay = !isPlay;
			if (isPlay) {
				play.text = "stop";
			}
			else {
				play.text = "play";
			}
		}
		else if (reload.isClicked()) {
			bool &isReload = owner->isReload;
			isReload = true;
		}
	}
};

void ConsoleSystem::lateInitialize() {
	ConsoleWidget *widget = new ConsoleWidget;
	widget->owner = this;
	auto widgetOverlay = Engine::instance()->renderSystem->getLayerAs<WidgetOverlay>("widget");
	*widgetOverlay << widget;
}
void ConsoleSystem::tick(const double &deltaTime) {
	
}

//struct Console {		
	//	WidgetOverlay &overlay;
	//	UI::Canvas canvas;
	//	UI::InputText input;
	//	Console(WidgetOverlay &overlay) : overlay(overlay)  {
	//		overlay << &canvas;
	//		canvas << &input;
	//	}
	//	void draw() {
	//		ImGui::Separator();	
	//		//char inputBuffer[256];
	//		//ImVector<char *> entries;
	//		//ImGui::Begin("Console");			
	//		//ImGui::TextWrapped("TESTTT");
	//		//
	//		//ImGui::Separator();
	//		//const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
	//		//ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar);
	//		//ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));
	//		//for (auto i : entries) {				
	//		//	ImGui::TextUnformatted(i);
	//		//}
	//		//ImGui::PopStyleVar();
	//		//ImGui::EndChild();
	//		//ImGui::Separator();
	//		//
	//		//ImGui::Button("input");
	//		//ImGui::SameLine();
	//		//
	//		//const ImGuiInputTextFlags input_text_flags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory;
	//		//if (ImGui::InputText("##inputBox", inputBuffer, IM_ARRAYSIZE(inputBuffer), input_text_flags, 
	//		//		[](ImGuiInputTextCallbackData* data)->int{
	//		//			Console *self = (Console*)data->UserData;						
	//		//			switch (data->EventFlag) {
	//		//				case ImGuiInputTextFlags_CallbackCompletion: {
	//		//
	//		//					break;
	//		//				}
	//		//				case ImGuiInputTextFlags_CallbackHistory: {
	//		//					break;
	//		//				}
	//		//			}
	//		//			return 0;
	//		//		}, this)) {
	//		//	char *c = new char[IM_ARRAYSIZE(inputBuffer)];
	//		//	strcpy(c, inputBuffer);
	//		//	entries.push_back(c);
	//		//	ImGui::SetKeyboardFocusHere(-1);
	//		//	strcpy(inputBuffer, "");
	//		//} 
	//		//ImGui::End();
	//
	//
	//	}
	//};
	//static Console console(*this);