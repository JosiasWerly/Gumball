#pragma once
#ifndef _sceneoverlay
#define _sceneoverlay

#include <Render/RenderOverlay.hpp>
#include <Render/Core/Mesh.hpp>
#include <Render/Core/Shaders.hpp>

#include <list>

class GMODULE ViewHandle {
    friend class SceneOverlay;

public:
    class GMODULE ViewMode {
    public:
        enum class eProjectionMode {
            Ortho, Perspective, Custom
        };
        eProjectionMode eProjection;
        glm::mat4 mProjection = glm::mat4(1.0f);

        ViewMode();
        void setProjectionPerspective(float fovy = glm::radians(45.0f), float aspect = 1.3333333730697632, float zNear = 0.1f, float zFar = 500.0f);
        void setProjectionOrtho(float left, float right, float bottom, float top, float zNear = .1f, float zFar = 200.0f);
        void setProjectionCustom(glm::mat4 customProj);
        const glm::mat4 &getProjection();
    };
    
    ViewMode viewMode;
    Transform *transform = nullptr;

    ViewHandle();
    virtual ~ViewHandle();

    void enable();
    void disable();
};
class GMODULE DrawHandle {
    friend class SceneOverlay;    
    ShaderInstance shaderInstance;
    MeshInstance meshInstance;

public:
    Transform *transform = nullptr;
    DrawHandle();
    ~DrawHandle();

    void enable();
    void disable();
    void setMesh(MeshData *newMesh);
    void setShader(Shader *newShader);

    //TODO: remove Instance fromt he name
    Inline ShaderInstance &getShaderInstance() { return shaderInstance; }
    Inline MeshInstance &getMeshInstance() { return meshInstance; }
};
class GMODULE FboHandle {
    struct Fbo *fbo;
    MeshInstance screenMesh;
    ShaderInstance shaderInstance;

public:
    FboHandle();
    ~FboHandle();
    
    void setShader(Shader *newShader);
    Inline ShaderInstance &getShader() { return shaderInstance; }
    Inline MeshInstance &getMesh() { return screenMesh; }

    void flush();
    void render();
};

class GMODULE SceneOverlay : public RenderOverlay {
private:
    friend class ViewHandle;
    friend class DrawHandle;

    list<ViewHandle *> views;
    list<DrawHandle *> draws;
    
    FboHandle *gbuffer;
    Shader *geometryShader;

public:
    void onAttach() override;
    void onDetach() override;
    void onRender(const double &deltaTime) override;
    string name() override { return "scene"; }
};
#endif // !_sceneoverlay