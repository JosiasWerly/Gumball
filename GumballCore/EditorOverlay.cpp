#include "EditorOverlay.hpp"

void EditorOverlay::onAttach() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    
    ImGui_ImplGlfw_InitForOpenGL(RenderSystem::instance().mainWindow.GetGLWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 440");
    ImGui::StyleColorsDark();
}
void EditorOverlay::onDetach() {
}
void EditorOverlay::onRender(float deltaTime) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Demo window");
    ImGui::Button("Hello!");
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}