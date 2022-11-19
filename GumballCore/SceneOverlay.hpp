#pragma once
#ifndef _sceneoverlay
#define _sceneoverlay
#include "Engine.hpp"
#include "RenderSystem.hpp"
#include "AssetManager.hpp"
#include "Mesh.hpp"
#include "Shaders.hpp"
#include "GLBuffer.hpp"

#include <list>

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
        const glm::mat4 &getProjection() {
            return mProjection;
        }
    };

    ViewMode viewMode;
    Transform transform;

    View();
    ~View();
};

class GBCORE DrawCallData {
    Vao *vao = nullptr;
    Vbo *vbo = nullptr;
    Ibo *ibo = nullptr;
    MeshData *meshData = nullptr;
public:
    DrawCallData(MeshData *mesh);
    ~DrawCallData();
    Inline void bind();
    Inline void unBind();
    Inline void draw();

    Inline MeshData *getMeshData() const { return meshData; }
};

class GBCORE DrawCallInstance {
    DrawCallData *drawData = nullptr;
public:
    Material material;
    Transform transform;

    DrawCallInstance(string meshName);
    ~DrawCallInstance();
};

class GBCORE SceneOverlay :
    public IRenderOverlay {
    friend class View;
    friend class DrawCallData;
    friend class DrawCallInstance;

    struct MeshDrawCallLayer {
        MeshDrawCallLayer(DrawCallData *data) :
            data(data) {
        }
        DrawCallData *data = nullptr;
        list<DrawCallInstance *> instances;
    };
    MeshDrawCallLayer *findDrawLayer(MeshData *meshData) {
        for (auto &dc : drawCallLayers) {
            if (dc.data->getMeshData() == meshData)
                return &dc;
        }
        return nullptr;
    }
    
    list<View *> views;
    list<MeshDrawCallLayer> drawCallLayers;
public:
    SceneOverlay();
    ~SceneOverlay();
    void onAttach() override;
    void onDetach() override;
    void onRender(const double &deltaTime) override;

    template<class T> T *instantiate() = delete;
    template<class T> T *instantiate(string) = delete;
    template<class T> void destroy(T*&) = delete;

};


#endif // !_sceneoverlay


//class GBCORE DrawInstance {
//    Vao *vao = nullptr;
//    Vbo *vbo = nullptr;
//    Ibo *ibo = nullptr;
//
//public:
//    MeshData *meshData = nullptr;
//    Material material;
//    Transform transform;
//
//    DrawInstance();
//    ~DrawInstance();
//    bool setMesh(string name);
//    bool setTexture(string name);
//
//    void bind();
//    void unbind();
//    void draw();
//};