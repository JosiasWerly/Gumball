#include "WidgetOverlay.hpp"
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>
#include <ImGui/imnodes.h>

#include "Core/GLUtils.hpp"
#include "Render.module.hpp"

WidgetOverlay::~WidgetOverlay() {
	for (auto *h : elements) {
		delete h;
	}	
}
void WidgetOverlay::onAttach() {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImNodes::CreateContext();
	guiIO = &ImGui::GetIO();
	guiIO->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	guiIO->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	ImGui_ImplGlfw_InitForOpenGL(window()->getGLWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 440");
	ImNodes::StyleColorsDark();
	ImGui::StyleColorsDark();
}
void WidgetOverlay::onDetach() {
	ImNodes::DestroyContext();
	ImGui::DestroyContext();
}
void WidgetOverlay::onRender(const double &deltaTime) {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	
	for (auto e : elements)
		e->render();

	//ImGui::Begin("simple node editor");
	//
	//ImNodes::BeginNodeEditor();
	//ImNodes::BeginNode(1);
	//
	//ImNodes::BeginNodeTitleBar();
	//ImGui::TextUnformatted("simple node :)");
	//ImNodes::EndNodeTitleBar();
	//
	//ImNodes::BeginInputAttribute(2);
	//ImGui::Text("input");
	//ImNodes::EndInputAttribute();
	//
	//ImNodes::BeginOutputAttribute(3);
	//ImGui::Indent(40);
	//ImGui::Text("output");
	//ImNodes::EndOutputAttribute();
	//
	//ImNodes::EndNode();
	//ImNodes::EndNodeEditor();

	ImGui::End();

	ImGui::ShowDemoWindow();

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
WidgetOverlay &WidgetOverlay::operator<<(UIDrawable *element) {
	elements.push_back(element);
	return *this;
}
WidgetOverlay &WidgetOverlay::operator>>(UIDrawable *element) {
	elements.remove(element);
	return *this;
}
