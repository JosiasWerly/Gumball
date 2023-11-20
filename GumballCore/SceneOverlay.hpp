#pragma once
#ifndef _sceneoverlay
#define _sceneoverlay
#include "Engine.hpp"
#include "RenderSystem.hpp"
#include "AssetManager.hpp"
#include "Mesh.hpp"
#include "Shaders.hpp"
#include "GLBuffer.hpp"
#include "EntitySystem.hpp"

#include <list>

class GBCORE ViewHandle {
    friend class SceneOverlay;
    SceneOverlay *scene = nullptr;
public:
    class ViewMode {
    public:
        enum class eProjectionMode {
            //projection = false ?//just for testing
            //glm::ortho(0.f, (float)x, 0.f, (float)y, -.1f, 100.f) :
            //glm::perspective(glm::radians(45.0f), (float)x/(float)y, .1f, 200.0f);

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
    Transform *transform = nullptr;

    ViewHandle();
    virtual ~ViewHandle();

    void enable();
    void disable();
    void setLayer(int newLayer);
};
class GBCORE DrawHandle {
    friend class SceneOverlay;
    SceneOverlay *scene = nullptr;
    
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

    Inline MeshInstance &getMeshInstance() { return meshInstance; }
    Inline ShaderInstance &getShaderInstance() { return shaderInstance; }
};
class GBCORE FboHandle {
    Fbo fbo;
    MeshInstance screenMesh;
    ShaderInstance shaderInstance;
public:
    FboHandle();
    ~FboHandle();
    
    void setShader(Shader *newShader);
    Inline ShaderInstance &getShader() { return shaderInstance; }
    Inline MeshInstance &getMesh() { return screenMesh; }
    Inline Fbo &getFbo() { return fbo; }
    
    void render();
};

class GBCORE SceneOverlay :
    public IRenderOverlay {
private:
    friend class ViewHandle;
    friend class DrawHandle;

    list<ViewHandle *> views;
    list<DrawHandle *> draws;

    FboHandle *gbuffer;
public:
    SceneOverlay();
    ~SceneOverlay();
    void onAttach() override;
    void onDetach() override;
    void onRender(const double &deltaTime) override;
};


//template<class TKey, class TInner>
//struct Layer {
//    typedef typename TKey tkey;
//    typedef typename TInner tinner;
//
//    TKey *key;
//    list<TInner *> values;
//
//    template<class tComparator, class tInstantiator>
//    TInner *find(tComparator fnxComp, tInstantiator fnxAdd) {
//        for (auto &i : values) {
//            if (fnxComp(i->key)) {
//                return i;
//            }
//        }
//        if (fnxAdd) {
//            TInner *out = fnxAdd();
//            values.push_back(out);
//            return out;
//        }
//        return nullptr;
//    }
//};
//
//Layer<int, Layer<ViewHandle, Layer<ShaderInstance, Layer<DrawCallBuffer, DrawHandle>>>> renderCache;

#endif // !_sceneoverlay