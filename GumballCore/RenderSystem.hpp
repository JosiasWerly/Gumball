#pragma once
#ifndef _rendersystem
#define _rendersystem

#include <iostream>
#include <list>
#include <unordered_map>
using namespace std;

#include "GLUtils.hpp"
#include "Subsystem.hpp"
#include "Patterns.hpp"
#include "InputSystem.hpp"
#include "Event.hpp"
#include "Math.hpp"


class SceneOverlay;
class WidgetOverlay;



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
    void setTitle(string newTitle);
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
    virtual void onRender(const double &deltaTime);
};

class RenderSystem :
    public EngineSystem {
    
    list<IRenderOverlay*> layers;
public:
    Window mainWindow;
    SceneOverlay *const scene;
    WidgetOverlay *const widget;

    RenderSystem();
    ~RenderSystem();
    void initialize() override;
    void lateInitialize() override;
    void shutdown() override;
    void tick(const double &deltaTime) override;
    void pushLayer(IRenderOverlay *layer, bool pushBack = true);
    void popLayer(IRenderOverlay *layer);
    //TODO:transform this into a tempalte method
    Inline IRenderOverlay *getLayer(string name) {
        for (auto &l : layers) {
            if (l->name == name)
                return l;
        }
        return nullptr;
    }
    Inline list<IRenderOverlay*>& getLayerList() { return layers; }
    template<class t> t *getLayerAs(string name) { return dynamic_cast<t *>(getLayer(name)); }

    ESystemTickType tickType() override { return ESystemTickType::all; }

    static void onWindowResize(GLFWwindow *window, int width, int height);
};


#endif // !_viewport
