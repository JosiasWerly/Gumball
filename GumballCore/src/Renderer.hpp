#ifndef _renderer
#define _renderer
#include "Gumball.hpp"
#include "GLBuffers.hpp"
#include "Drawcall.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"
#include "Transform.hpp"
#include "Camera.hpp"




class Renderer {
    Gumball::Window* gWindow;
public:
    GLFWwindow* window;
    set<iDrawCall*> drawcalls;
    Camera* camera;
    
    
    Renderer(Gumball::Window* target) {
        this->gWindow = target;
        this->window = target->getWindow();
    }
    void drawRender() {
        for (auto& d : drawcalls) {
            d->sa.params.get<glm::mat4>("uProj") = camera->viewMode.mProjection;
            d->sa.params.get<glm::mat4>("uView") = camera->transform.getModel();
            
            d->draw(*this);

            d->sa.params.uploadParams();
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
        auto& am = AssetManager::instance();
        auto meshData = am.assets[objName]->content.get<MeshData>();
        sa.changeShader("default");
        vl.push<float>(3);
        vl.push<float>(3);
        vl.push<float>(2);
        va.bind();
        vb.setData(meshData.mesh.data(), (unsigned int)meshData.mesh.size() * sizeof(MeshVertexData));
        ib.setData((unsigned int*)meshData.index.data(), (unsigned int)meshData.index.size());
        va.addBuffer(vb, vl);
        va.unbind();
    }
    void draw(const class Renderer& renderer) {
        va.bind();
        sa.bind();
        sa.params.get<glm::mat4>("uModel") = transform.getResultModel();        
        glDCall(glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr));
    }
};
#endif // !_renderer