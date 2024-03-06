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
	void create(string name, Vector2i size);
    void clearRender();
    void render();
    void setSize(Vector2i size);
    const Vector2i& getSize();
    GLFWwindow* getGLWindow();
    void setTitle(string newTitle);
};

class GBCORE RenderOverlay {
protected:
    RenderOverlay(string name) :
        name(name) {
    }
public:
    const string name;
    virtual ~RenderOverlay() = default;
    virtual void onAttach();
    virtual void onDetach();
    virtual void onRender(const double &deltaTime);
};

class RenderSystem :
    public EngineSystem {
    
    list<RenderOverlay*> overlays;
    SceneOverlay *const scene;
    WidgetOverlay *const widget;
public:
    Window mainWindow;

    RenderSystem();
    ~RenderSystem();
    void initialize() override;
    void lateInitialize() override;
    void shutdown() override;
    void tick(const double &deltaTime) override;
    
    void pushOverlay(RenderOverlay *overlay, bool front = false);
    void popOverlay(RenderOverlay *overlay);
    RenderOverlay *getOverlay(string name);
    template<class t> t *getOverlay() { 
        for (auto i : overlays) {
            if (t *ti = dynamic_cast<t *>(i)) {
                return ti;
            }
        };
        return nullptr;
    }
    template<class t> t *getOverlay(string name) { return dynamic_cast<t *>(getOverlay(name)); }

    Inline SceneOverlay *getScene() const { return scene; }
    Inline WidgetOverlay *getWidget() const { return widget; }
    Inline list<RenderOverlay *> getLayerList() const { return overlays; }
    ESystemTickType tickType() override { return ESystemTickType::all; }

    static void onWindowResize(GLFWwindow *window, int width, int height);
};


#endif // !_rendersystem
