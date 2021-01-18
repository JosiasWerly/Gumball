#include "Gumball.hpp"
#include "lang.hpp"



int main() {
	Engine& engine = Engine::instance();
	engine.setup();
	engine.window->config("Gameplay test", 800, 600);
	engine.assetManager->loadAssets("res/");
	engine.tick();
	return 0;
}






//void Imgui::initialize(GLFWwindow* window) {
//    IMGUI_CHECKVERSION();
//    ImGui::CreateContext();
//    ImGui::StyleColorsDark();
//    ImGui_ImplGlfw_InitForOpenGL(window, true);
//    ImGui_ImplOpenGL3_Init("#version 130");
//}
//void Imgui::shutdown() {
//    ImGui_ImplGlfw_Shutdown();
//    ImGui::DestroyContext();
//}
//void Imgui::requestNewFrame() {
//    ImGui_ImplOpenGL3_NewFrame();
//    ImGui_ImplGlfw_NewFrame();
//    ImGui::NewFrame();
//    ///just some test
//    {
//        static float f = 0.0f;
//        static int counter = 0;
//
//        ImGui::Begin("Hello, world!");                              // Create a window called "Hello, world!" and append into it.
//
//        ImGui::Text("This is some useful text.");                   // Display some text (you can use a format strings too)
//        //ImGui::Checkbox("Demo Window", &show_demo_window);        // Edit bools storing our window open/close state
//        //ImGui::Checkbox("Another Window", &show_another_window);
//
//        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);                // Edit 1 float using a slider from 0.0f to 1.0f
//        //ImGui::ColorEdit3("clear color", (float*)&clear_color);   // Edit 3 floats representing a color
//
//        if (ImGui::Button("Button"))                                // Buttons return true when clicked (most widgets return true when edited/activated)
//            counter++;
//        ImGui::SameLine();
//        ImGui::Text("counter = %d", counter);
//
//        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
//        ImGui::End();
//    }
//}
//void Imgui::disposeFrame() {
//    ImGui::Render();
//    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
//}




/*
##### Gumball 0.1 #####
refator:
[mk3] render and window relation
[mk2] asset system, that can support runtime load
[mk1] project sample
[mk1] separe gamethread vs drawThread && querrySystem

[todo] collision detection
[todo] traces, lines
*/