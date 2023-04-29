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
    delete mesh;
}
void DrawHandle::enable() {
    scene->draws.push_back(this);
}
void DrawHandle::disable() {
    scene->draws.remove(this);
}
void DrawHandle::setMesh(MeshData *newMesh) {
    if (mesh)
        delete mesh;
    mesh = new MeshBuffer(newMesh);
}
void DrawHandle::setShader(Shader *newShader) {
    auto as = Engine::instance()->assetSystem;
    shaderInstance.setShader(newShader);
    if (newShader) {
        shaderInstance.bind();

        auto &io = shaderInstance.uniformIO();
        Tbo &tex = as->getAsset("logo")->getContent().pin<Image>()->getTexture();
        Tbo &tex2 = as->getAsset("scotty")->getContent().pin<Image>()->getTexture();
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

}
void SceneOverlay::onDetach() {

}
void SceneOverlay::onRender(const double &deltaTime) {
	for (auto &v : views) {
		const auto mView = v->transform->getMat();
		for (auto &d : draws) {
            auto &mesh = d->mesh;
			auto &shader = d->shaderInstance;
            auto &shaderIO = shader.uniformIO();
            
            mesh->bind();
            shader.bind();
            shaderIO.setParam<glm::mat4>("uView", mView);
            shaderIO.setParam<glm::mat4>("uProj", v->viewMode.mProjection);
            shaderIO.setParam<glm::mat4>("uModel", d->transform->getMat());
            shaderIO.upload();
            mesh->draw();
            shader.unbind();
		}
	}
}