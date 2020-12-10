#pragma once
#ifndef _drawables
#define _drawables
#include "Renderer.hpp"
#include "Texture.hpp"
#include "Mesh.hpp"


class Meshdata :
    public iDrawCall {
public:
    Transform transform;
    Meshdata() {}
    void loadMesh(string objName) {
        auto& am = AssetManager::instance();
        auto meshData = am.getData<MeshData>(objName);
        sa.changeShader("default");
        sa.params.get<glm::vec3>("uColor") = glm::vec3(1, 1, 1);

        vl.push<float>(3);//pos
        vl.push<float>(3);//normal 
        vl.push<float>(2);//UV
        va.bind();
        vb.setData(meshData.mesh.data(), (unsigned int)meshData.mesh.size() * sizeof(MeshVertexData));
        ib.setData((unsigned int*)meshData.index.data(), (unsigned int)meshData.index.size());
        va.addBuffer(vb, vl);
        va.unbind();
    }
    void setColor(glm::vec3 color) {
        sa.params.get<glm::vec3>("uColor") = color;
    }
    void draw(const class Renderer& renderer) {
        va.bind();
        sa.bind();
        sa.params.get<glm::mat4>("uModel") = transform.getResultModel();
        glDCall(glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr));
    }
};
class Sprite :
    public iDrawCall {
public:
    Transform transform;
    Sprite() {}
    void loadSprite(string objName) {
        auto& am = AssetManager::instance();
        auto meshData = am.getData<MeshData>(objName);
        sa.changeShader("default");
        //vl.push<float>(3);
        //vl.push<float>(3);
        //vl.push<float>(2);
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
#endif // !_assetManger