#pragma once
#ifndef _viewport
#include <iostream>
#include <list>
using namespace std;

#include "GLUtils.hpp"
#include "EngineSystem.hpp"
#include "Patterns.hpp"
#include "Drawable.hpp"
#include "Math.hpp"

class MSCounter {
    double lastTime = 0;
    double ms = 0;
    unsigned frames = 0;
public:
    MSCounter() {
        lastTime = glfwGetTime();
    }
    void tick() {
        double actualTime = glfwGetTime();
        frames++;
        if (actualTime - lastTime >= 1.0) {
            ms = 1000 / frames;
            frames = 0;
            lastTime += 1.0f;
        }
    }
    const double& getMS() const {
        return ms;
    }
};

class Window {
    MSCounter msCounter;
    GLFWwindow* window = nullptr;
    Vector2i winSize;
public:
	void create(string Name, Vector2i size);   

    void clearRender();
    void render();

    const double& getMS();
    void setSize(Vector2i size);
    const Vector2i& getSize();
    GLFWwindow* getWindowReference();
};

struct DrawOrder {
    Material material;
    list<DrawInstance*> drawInstances;
};

class RenderSystem :
    public IEngineSystem,
    public Singleton<RenderSystem> {


    
public:
    list<DrawInstance*> drawInstances;
    Window mainWindow;
    void initialize() override;
    void shutdown() override;
    void tick() override;
};

#endif // !_viewport
