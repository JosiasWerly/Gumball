#ifndef _window
#define _window



#include "GLBuffers.hpp"
#include "GFunctionLibrary.hpp"


class FpsCounter {
    double lastTime = 0;
    unsigned int frames = 0;
public:
    double ms = 0;
    FpsCounter() {
        lastTime = glfwGetTime();
    }
    void fpsTick() {
        double actualTime = glfwGetTime();
        frames++;
        if (actualTime - lastTime >= 1.0) {
            ms = 1000 / frames;
            frames = 0;
            lastTime += 1.0f;
        }
    }
    double getMsBySec() {
        return ms;
    }
};



class Window {
    int x, y;
    GLFWwindow* window;
    FpsCounter fpsCounter;
public:
    Window();
    ~Window();

    GLFWwindow* getGLFWindow();
    void clearGLStack();
    float getAspec();
    void create(string winName, int x, int y);
    void config(string winName, int x, int y);
    void setSize(int x, int y);

    void clearBuffer();
    void swapBuffers();
    bool shouldClose();
    double getMS();
};

#endif // !_window
