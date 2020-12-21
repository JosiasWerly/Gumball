#pragma once
#ifndef _drawables
#define _drawables
#include "Renderer.hpp"
#include "Texture.hpp"
#include "Mesh.hpp"

class MeshDrawable :
    public Drawable {
public:
    void changeMesh(string objName) {
        auto& am = AssetManager::instance();
        auto meshData = am.getData<MeshData>(objName);
        sa.changeShader("default");
        sa.params.get<glm::vec4>("uColor") = glm::vec4(1, 1, 1, 1);
        //tex.changeTexture("gumbalA");
        //for (int x = 0; x < 64; x++)
        //    tex.setPixel(x, x, 0xff0000ff);
        //tex.udpToGPU();
        sa.params.get<int>("uTexture") = 0;

        vl.push<float>(3);//pos
        vl.push<float>(3);//normal 
        vl.push<float>(2);//UV
        va.bind();
        vb.setData(meshData.mesh.data(), (unsigned int)meshData.mesh.size() * sizeof(MeshVertexData));
        ib.setData((unsigned int*)meshData.index.data(), (unsigned int)meshData.index.size());
        va.addBuffer(vb, vl);
        va.unbind();
    }
    void draw(const class Renderer& renderer) {
        va.bind();
        //tex.bind();
        sa.bind();
        sa.params.uploadParams();
        sa.params.get<glm::mat4>("uModel") = transform->getResultModel();
        glDCall(glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr));
    }
};
#endif // !_assetManger