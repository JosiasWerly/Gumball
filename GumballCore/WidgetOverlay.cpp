#include "WidgetOverlay.hpp"
#include "Engine.hpp"


WidgetOverlay::WidgetOverlay() :
	RenderOverlay("widget") {
}
void WidgetOverlay::onAttach() {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	guiIO = &ImGui::GetIO();
	//guiIO->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	guiIO->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	guiIO->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	auto &RenderSys = *Engine::instance()->renderSystem;
	ImGui_ImplGlfw_InitForOpenGL(RenderSys.mainWindow.getGLWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 440");
	ImGui::StyleColorsDark();

}
void WidgetOverlay::onDetach() {

}
void WidgetOverlay::onRender(const double &deltaTime) {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	
	for (auto e : elements)
		e->render(deltaTime);

	//ImGui::ShowDemoWindow();

	ImGui::Render();	
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	
	// Update and Render additional Platform Windows
	// (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
	//  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
	if (guiIO->ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		GLFWwindow *backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}
}
WidgetOverlay &WidgetOverlay::operator<<(Widget *element) {
	elements.push_back(element);
	return *this;
}
WidgetOverlay &WidgetOverlay::operator>>(Widget *element) {
	elements.remove(element);
	return *this;
}

void Widget::render(const double &deltaTime) {
}
void Widget::setVisibility(bool newVisible) {
	isVisible = newVisible;
}

void WidgetContainer::render(const double &deltaTime) {
	if (!getVisibility())
		return;
	beginDraw();
	for (auto c : children)
		c->render(deltaTime);
	endDraw();
}
void WidgetContainer::beginDraw() {
	ImGui::Begin(getName().c_str());
}
void WidgetContainer::endDraw() {
	ImGui::End();
}
WidgetContainer &WidgetContainer::operator<<(Widget *child) {
	if (child->parent)
		(*child->parent) >> child;
	children.push_back(child);
	child->parent = this;
	return *this;
}
WidgetContainer &WidgetContainer::operator>>(Widget *child) {
	children.remove(child);
	child->parent = nullptr;
	return *this;
}
void WidgetContainer::addChildren(list<Widget *> children) {
	for (auto it : children) {
		(*this) << it;
	}
}
void WidgetContainer::delChildren(list<Widget *> children) {
	for (auto it : children) {
		(*this) >> it;
	}
}

UserWidget::UserWidget() {
	setVisibility(false);
}
void UserWidget::show() {
	if (getVisibility())
		return;

	WidgetOverlay &wOverlay = *Engine::instance()->renderSystem->getWidget();
	wOverlay << this;
	setVisibility(true);
}
void UserWidget::hide() {
	if (!getVisibility())
		return;

	WidgetOverlay &wOverlay = *Engine::instance()->renderSystem->getWidget();
	wOverlay >> this;
	setVisibility(false);
}



namespace UI {
	void Text::render(const double &deltaTime) {
		ImGui::Text(text.c_str());
	}

	void InputText::render(const double &deltaTime) {
		const ImGuiInputTextFlags inFlags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory;
		const auto InputCallback = [](ImGuiInputTextCallbackData *data)->int {
			InputText *self = static_cast<InputText*>(data->UserData);
			switch (data->EventFlag) {
				case ImGuiInputTextFlags_CallbackCompletion: {
					break;
				}
				case ImGuiInputTextFlags_CallbackHistory: {
					break;
				}
			}
			return 0;
		};
		if (ImGui::InputText("Input", inBuffer, IM_ARRAYSIZE(inBuffer), inFlags, InputCallback, this)) {
			str = string(inBuffer);			
			ImGui::SetKeyboardFocusHere(-1);
			strcpy(inBuffer, "");
		}
	}
	bool InputText::getInput(string &out) const {
		out = str;
		return str.size();
	}

	void Button::render(const double &deltaTime) {
		ImGui::Button(text.c_str());
		clicked = ImGui::IsItemClicked(0);
	}
};