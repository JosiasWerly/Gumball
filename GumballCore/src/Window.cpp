#include "Window.hpp"
#include "InputManager.hpp"

void Imgui::initialize(GLFWwindow* window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
}
void Imgui::shutdown() {
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
void Imgui::requestNewFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ///just some test
    {
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Hello, world!");                              // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text.");                   // Display some text (you can use a format strings too)
        //ImGui::Checkbox("Demo Window", &show_demo_window);        // Edit bools storing our window open/close state
        //ImGui::Checkbox("Another Window", &show_another_window);

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);                // Edit 1 float using a slider from 0.0f to 1.0f
        //ImGui::ColorEdit3("clear color", (float*)&clear_color);   // Edit 3 floats representing a color

        if (ImGui::Button("Button"))                                // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
    }
}
void Imgui::disposeFrame() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}



Window::Window() {
    
}
Window::~Window() {
    glfwDestroyWindow(window);
    glfwTerminate();
}
GLFWwindow* Window::getGLFWindow() {
    return window;
}
float Window::getAspec() {
    return ((float)x / (float)y);
}
void Window::clearGLStack() {
    glUseProgram(0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
void Window::create(string winName, int x, int y) {
    this->x = x;
    this->y = y;
    glfwInit();
    //cout << "glVersion " << glGetString(GL_VERSION) << endl;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //profile

    window = glfwCreateWindow(x, y, winName.c_str(), NULL, NULL);

    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGL())
        cout << "fail to load window" << endl;

    glViewport(0, 0, x, y);
    glEnable(GL_DEPTH_TEST);
    glfwSwapInterval(1);


    ///////////////////Old stuff//////////////////////
    //textureAlpha
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //endTextureAlpha

    //TODO: decent Input manager
    Input::setFocus(*this);

    imgui.initialize(window);
}
void Window::setSize(int x, int y) {
    glfwSetWindowSize(window, x, y);
    glViewport(0, 0, x, y);
}
void Window::config(string winName, int x, int y) {
    glfwSetWindowTitle(window, winName.c_str());
    glfwSetWindowSize(window, x, y);
    glViewport(0, 0, x, y);
}
void Window::clearBuffer() {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glfwPollEvents();
    Input::poolEvents();
    
}
void Window::swapBuffers() {
    imgui.requestNewFrame();
    imgui.disposeFrame();
    glfwSwapBuffers(window);
    fpsCounter.fpsTick();
}
bool Window::shouldClose() {
    return glfwWindowShouldClose(window);
    
}
double Window::getMS() {
    return fpsCounter.getMsBySec();
}