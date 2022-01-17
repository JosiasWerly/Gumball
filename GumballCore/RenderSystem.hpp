#pragma once
#ifndef _viewport
#include <iostream>
#include <list>
#include <unordered_map>

using namespace std;

#include "GLUtils.hpp"
#include "EngineSystem.hpp"
#include "Patterns.hpp"
#include "Drawable.hpp"
#include "InputSystem.hpp"
#include "Math.hpp"

class TimeStats {
    double lastRegister = 0;
    
    double ms = 0;
    double deltaTime = 0;

    double fpsTime = 0;
    unsigned frames = 0;
public:
    TimeStats() {
        lastRegister = glfwGetTime();
    }

    void capture() {
        const double actualTime = glfwGetTime();
        const double delta = actualTime - lastRegister;

        ms = delta * 1000.0;
        deltaTime = delta / 1000.0;

        fpsTime += delta;
        if (fpsTime >= 1000.) {
            fpsTime = 0;
            frames = 0;
        }
        else
            frames++;
    }
    Inline const double& getDeltaTime() const {
        return deltaTime;
    }
    Inline const double& getMS() const {
        return ms;
    }
    Inline char getFPS() const {
        return char(1000/ms);
    }
};




class Window {
    GLFWwindow* window = nullptr;
    Vector2i winSize;
public:
	void create(string Name, Vector2i size);   

    void clearRender();
    void render();

    void setSize(Vector2i size);
    const Vector2i& getSize();
    GLFWwindow* GetGLWindow();
};



class View {
public:
    //TODO: refactor ViewMode class
    class ViewMode {
    public:
        //projection = false ?//just for testing
        //glm::ortho(0.f, (float)x, 0.f, (float)y, -.1f, 100.f) :
        //glm::perspective(glm::radians(45.0f), (float)x/(float)y, .1f, 200.0f);

        enum class eProjectionMode {
            Ortho, Perspective, Custom
        };
        eProjectionMode eProjection;
        glm::mat4 mProjection = glm::mat4(1.0f);

        ViewMode() {
            setProjectionPerspective();
        }
        void setProjectionPerspective(float fovy = glm::radians(45.0f), float aspect = 1.3333333730697632, float zNear = 0.1f, float zFar = 500.0f) {
            mProjection = glm::perspective(fovy, aspect, zNear, zFar);
            eProjection = eProjectionMode::Perspective;
        }
        void setProjectionOrtho(float left, float right, float bottom, float top, float zNear = .1f, float zFar = 200.0f) {
            mProjection = glm::ortho(left, right, bottom, top, zNear, zFar);
            eProjection = eProjectionMode::Perspective;
        }
        void setProjectionCustom(glm::mat4 customProj) {
            mProjection = customProj;
            eProjection = eProjectionMode::Custom;
        }
        const glm::mat4& getProjection() {
            return mProjection;
        }
    };

    ViewMode viewMode;
    Transform transform;
};


class IRenderLayer {
public:
    const string name;
    list<View*> views;
    list<DrawInstance*> drawInstances;


    IRenderLayer(string name) : 
        name(name) {
    }
    virtual ~IRenderLayer() = default;
    virtual void onAttach() {}
    virtual void onDetach() {}
    virtual void onRender(float deltaTime);
};


class RenderSystem :
    public IEngineSystem,
    public Singleton<RenderSystem> {
    
    list<IRenderLayer*> layers;
public:
    Window mainWindow;

    ~RenderSystem() {
        for (auto &l : layers)
            delete l;
    }
    void initialize() override;
    void shutdown() override;
    void tick() override;

    void pushView(int layerId, View* view) {
        if (layers.size() > layerId)
            (layers.front() + layerId)->views.push_back(view);
    }
    void popView(int layerId, View* view) {
        if (layers.size() > layerId)
            (layers.front() + layerId)->views.remove(view);
    }
    void pushDrawInstance(int layerId, DrawInstance* drawInstance) {
        if (layers.size() > layerId)
            (layers.front() + layerId)->drawInstances.push_back(drawInstance);
    }
    void popDrawInstance(int layerId, DrawInstance* drawInstance) {
        if (layers.size() > layerId)
            (layers.front() + layerId)->drawInstances.remove(drawInstance);
    }
    void pushLayer(IRenderLayer *layer, bool pushBack = true) {
        if(pushBack)
            layers.emplace_back(layer);
        else
            layers.emplace_front(layer);
        layer->onAttach();
    }
    Inline list<IRenderLayer*>& getLayerList() { return layers; }
};

#endif // !_viewport
