#include "Renderer.hpp"

Window::Window() {

}
Window::~Window() {
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
}
void Window::setSize(int x, int y) {
    glfwSetWindowSize(window, x, y);
}
void Window::clearBuffer() {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void Window::swapBuffers() {
    glfwSwapBuffers(window);
    glfwPollEvents();
    Input::poolEvents();
    fpsCounter.fpsTick();
}
bool Window::shouldClose() {
    return glfwWindowShouldClose(window);
}
double Window::getMS() {
    return fpsCounter.getMsBySec();
}