#ifndef _renderer
#define _renderer
#include "Gumball.hpp"
#include "GLBuffers.hpp"
#include "Drawcall.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"
#include "Transform.hpp"
#include "Camera.hpp"




class iRenderProcess{
public:
    const class Renderer* render;
    virtual void process() = 0;
};

class Renderer {
    Gumball::Window* gWindow;
public:
    GLFWwindow* window;
    set<iDrawCall*> drawcalls;
    list<Camera*> cameras;
    Renderer(Gumball::Window* target) {
        this->gWindow = target;
        this->window = target->getWindow();
    }
    void drawRender() {
        for (auto& d : drawcalls) {
            drawByLayer(*d);
            //d->draw(*this);
        }
    }
    void clearRender() {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    void swapBuffers() {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    Camera* newCamera(unsigned char layer) {
        Camera *newCam = new Camera;
        newCam->viewMode.setProjectionPerspective(glm::radians(45.0f), gWindow->getAspec(), .1f, 200.0f);
        newCam->layer(layer);
        cameras.push_back(newCam);
        return newCam;
    }
    void drawByLayer(iDrawCall &d) {
        for (auto cam : cameras){
            if (cam->layer() & d.layer) {
                d.sa.getParam("uProj")->value<Uniform<glm::mat4>>().data = cam->viewMode.mProjection;
                d.sa.getParam("uView")->value<Uniform<glm::mat4>>().data = cam->transform.getModel();
                d.draw(*this);
            }
        }
    }
    Renderer& operator<<(iDrawCall* drawCall) {
        drawcalls.insert(drawCall);
        return *this;
    }
    Renderer& operator>>(iDrawCall* drawCall) {
        auto i = drawcalls.find(drawCall);
        if (i != drawcalls.end())
            drawcalls.erase(i);
    }
};



class Meshdata :
    public iDrawCall {
public:    
    Transform transform;
    Meshdata() {}
    void loadMesh(string objName) {
        sa.changeShader("default");
        auto& meshData = MeshSystem::instance().getAsset(objName);
        vl.push<float>(3);
        vl.push<float>(3);
        vl.push<float>(2);
        va.bind();
        vb.setData(meshData.mesh.data(), meshData.mesh.size() * sizeof(MeshVertexData));
        ib.setData((unsigned int*)meshData.index.data(), meshData.index.size());
        va.addBuffer(vb, vl);
        va.unbind();
    }
    void draw(const class Renderer& renderer) {
        va.bind();
        sa.bind();
        sa.getParam("uModel")->value<Uniform<glm::mat4>>().data = transform.getResultModel();
        //sa.getParam("uView")->value<Uniform<glm::mat4>>().data = renderer.viewMode.mView;

        sa.uploadParams();
        glDCall(glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr));
    }
};
#endif // !_renderer