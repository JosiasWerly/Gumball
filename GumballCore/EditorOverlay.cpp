#include "EditorOverlay.hpp"

void EditorOverlay::onAttach() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    guiIO = &ImGui::GetIO();
    //guiIO->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    guiIO->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    guiIO->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    
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

    ImGui::Begin("Debug");
    for (auto &kv : msStats) {
        ImGui::Text((kv.first + " : %.4f").c_str(), kv.second);
    }
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
    //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
    if (guiIO->ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}