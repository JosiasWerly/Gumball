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

void UIElement::render() {
	if (!isVisible)
		return;
	begin();
	draw();
	for (auto c : children)
		c->render();
	end();
}
void UIElement::begin() {}
void UIElement::end() {}
void UIElement::draw() {}
void UIElement::hide() {
	isVisible = false;
}
void UIElement::show() {
	isVisible = true;
}
UIElement &UIElement::operator<<(UIElement *child) {
	if (child->parent)
		(*child->parent) >> child;
	children.push_back(child);
	child->parent = this;
	return *this;
}
UIElement &UIElement::operator>>(UIElement *child) {
	children.push_back(child);
	child->parent = this;
	return *this;
}
void UIElement::addChildren(list<UIElement *> children) {
	this->children.merge(children);
}
void UIElement::delChildren(list<UIElement *> children) {
	throw; //TODO
}




namespace UI {
	void Canvas::begin() {
		ImGui::Begin(windowName.c_str());
	}
	void Canvas::end() {
		ImGui::End();
	}

	void Text::draw() {
		ImGui::Text(text.c_str());
	}
};



//ImGui::Begin("Debug");

	/*static bool clicked;
	static string text = "bt1";


	ImGui::PushID(0xa);

	if (clicked) {
		text = "bt1";
	}
	else {
		text = "clicked";
	}
	int i = clicked * 4;
	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(i / 7.0f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(i / 7.0f, 0.7f, 0.7f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(i / 7.0f, 0.8f, 0.8f));
	ImGui::Button(text.c_str());
	if (ImGui::IsItemClicked(0))
		clicked = !clicked;
	ImGui::PopStyleColor(3);
	ImGui::PopID();*/


	//for (auto &kv : msStats) {
	//    ImGui::Text((kv.first + " : %.4f").c_str(), kv.second);
	//}
	//ImGui::End();