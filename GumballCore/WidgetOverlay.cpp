#include "WidgetOverlay.hpp"
#include "Engine.hpp"


WidgetOverlay::WidgetOverlay() :
	IRenderOverlay("widget") {
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
		e->render();

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

void Widget::render() {
	if (!isVisible)
		return;
	begin();
	draw();
	for (auto c : children)
		c->render();
	end();
}
void Widget::begin() {}
void Widget::end() {}
void Widget::draw() {}
void Widget::hide() {
	isVisible = false;
}
void Widget::show() {
	isVisible = true;
}
Widget &Widget::operator<<(Widget *child) {
	if (child->parent)
		(*child->parent) >> child;
	children.push_back(child);
	child->parent = this;
	return *this;
}
Widget &Widget::operator>>(Widget *child) {
	children.push_back(child);
	child->parent = this;
	return *this;
}
void Widget::addChildren(list<Widget *> children) {
	this->children.merge(children);
}
void Widget::delChildren(list<Widget *> children) {
	throw; //TODO
}




namespace UI {
	void Canvas::begin() {
		ImGui::Begin(reinterpret_cast<const char*>(this));
	}
	void Canvas::end() {
		ImGui::End();
	}

	void Text::draw() {
		ImGui::Text(text.c_str());
	}

	void InputText::draw() {
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

	void Button::draw() {
		ImGui::Button(text.c_str());
		clicked = ImGui::IsItemClicked(0);
	}
};