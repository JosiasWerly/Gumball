#include "SceneOverlay.hpp"
#include "GLBuffer.hpp"

ViewHandle::ViewHandle() {
    scene = Engine::instance()->renderSystem->scene;
}
ViewHandle::~ViewHandle() {
    disable();
}
void ViewHandle::enable() {
    scene->views.push_back(this);
}
void ViewHandle::disable() {
    scene->views.remove(this);
}


DrawHandle::DrawHandle() {
    scene = Engine::instance()->renderSystem->scene;
}
DrawHandle::~DrawHandle() {
    disable();
}
void DrawHandle::enable() {
    scene->draws.push_back(this);
}
void DrawHandle::disable() {
    scene->draws.remove(this);
}
void DrawHandle::setMesh(MeshData *newMesh) {
    staticMesh.setMeshData(newMesh);
}
void DrawHandle::setShader(Shader *newShader) {
    auto as = Engine::instance()->assetSystem;
    shaderInstance.setShader(newShader);
    if (newShader) {
        shaderInstance.bind();
        auto &io = shaderInstance.uniformIO();        
        Tbo &tex = as->getContent<Image>("logo")->getTexture();
        Tbo &tex2 = as->getContent<Image>("scotty")->getTexture();
        io.setParam<Tbo *>("uTexture", &tex);
        io.setParam<Color>("uColor", 0xffffffff);
        io.upload();
        shaderInstance.unbind();
    }
}


SceneOverlay::SceneOverlay() : 
	IRenderOverlay("scene") {
}
SceneOverlay::~SceneOverlay() {
}
void SceneOverlay::onAttach() {
    auto &as = Engine::instance()->assetSystem;


    fbo = new Fbo;
    fbo->bind();
    fbo->addTexture();
    fbo->updateBuffers();
    fbo->unbind();
    
    plane = new DrawableBuffer;
    plane->bindAll();
    {
        MeshData meshData;

        meshData.mesh = {
            { {-1.f, -1.f, 0.0f},   {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f} },
            { {1.f,  -1.f, 0.0f},   {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f} },
            { {1.f,   1.f, 0.0f},   {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f} },
            { {-1.f,  1.f, 0.0f},   {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f} }
        };
        meshData.index = { 0, 1, 2, 2, 3, 0};
        VboBuilder()
            .setBuffer<void>(meshData.mesh.data(), (unsigned)meshData.mesh.size() * sizeof(MeshVertexData))
            .addAttrib<float>(3)//pos
            .addAttrib<float>(3)//normal
            .addAttrib<float>(2)//uv
            .build();
        auto &ibo = plane->getIbo();
        ibo.setBuffer(
            meshData.index.data(),
            static_cast<unsigned int> (meshData.index.size() * sizeof(unsigned)));
    }
    plane->unbindAll();

    fboShader.setShader(as->getContent<Shader>("planeShader"));
    auto io = fboShader.uniformIO();
    io.setParam<Tbo *>("uTexture", fbo->textures[0]);
}
void SceneOverlay::onDetach() {

}
void SceneOverlay::onRender(const double &deltaTime) {
	fbo->bind(EFboTarget::Write);
    fbo->cleaBuffer();
	for (auto &v : views) {
		const auto mView = v->transform->getMat();
		for (auto &d : draws) {
			auto &mesh = d->staticMesh;
			auto &shader = d->shaderInstance;
			auto &shaderIO = shader.uniformIO();

			mesh.bind();
			shader.bind();
			shaderIO.setParam<glm::mat4>("uView", mView);
			shaderIO.setParam<glm::mat4>("uProj", v->viewMode.mProjection);
			shaderIO.setParam<glm::mat4>("uModel", d->transform->getMat());
			shaderIO.upload();
			mesh.draw();
			shader.unbind();
		}
	}
	fbo->unbind();

    fbo->bind(EFboTarget::Read);
    plane->bind();
    fboShader.bind();
    fboShader.upload();
    plane->draw();
    
    
    //fbo->bind(EFboTarget::Read);
    //glBindFramebuffer(static_cast<unsigned>(EFboTarget::Write), 0);
    //glBlitFramebuffer(0, 0, 800, 600, 0, 0, 800, 600, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    //glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    //glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}