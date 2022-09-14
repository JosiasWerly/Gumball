#pragma once
#ifndef _rendersystem
#define _rendersystem

#include <iostream>
#include <list>
#include <unordered_map>
using namespace std;

#include "GLUtils.hpp"
#include "EngineSystem.hpp"
#include "Patterns.hpp"
#include "InputSystem.hpp"
#include "Math.hpp"


class Window {
    GLFWwindow* window = nullptr;
    Vector2i winSize;
public:
	void create(string Name, Vector2i size);   

    void clearRender();
    void render();

    void setSize(Vector2i size);
    const Vector2i& getSize();
    GLFWwindow* getGLWindow();
};

class GBCORE IRenderOverlay {
protected:
    IRenderOverlay(string name) :
        name(name) {
    }
public:
    const string name;
    virtual ~IRenderOverlay() = default;
    virtual void onAttach();
    virtual void onDetach();
    virtual void onRender(float deltaTime);
};

class RenderSystem :
    public IEngineSystem {
    
    list<IRenderOverlay*> layers;
public:
    Window mainWindow;
    
    ~RenderSystem() {
        for (auto &l : layers)
            delete l;
    }
    void initialize() override;
    void shutdown() override;
    void tick(float deltaTime) override;   
    void pushLayer(IRenderOverlay *layer, bool pushBack = true);
    void popLayer(IRenderOverlay *layer);

    Inline IRenderOverlay *getLayer(string name) {
        for (auto &l : layers) {
            if (l->name == name)
                return l;
        }
        return nullptr;
    }
    Inline list<IRenderOverlay*>& getLayerList() { return layers; }
    Inline GLADloadproc glProcAddres() { return (GLADloadproc)glfwGetProcAddress; }
};

#endif // !_viewport
