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



class View {
public:
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

struct DrawBoard
{
    list<View*> views;
    list<DrawInstance*> drawInstances;
};

class RenderSystem :
    public IEngineSystem,
    public Singleton<RenderSystem> {
public:
    Window mainWindow;
    //TODO: LAYER
    //unordered_map<char, DrawBoard> drawboards;
    DrawBoard drawboard;
    void initialize() override;
    void shutdown() override;
    void tick() override;
};

#endif // !_viewport
