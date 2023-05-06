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
    fbo = new Fbo;
    fbo->bind();
    fbo->addTexture();
    fbo->updateBuffers();
    fbo->unbind();


    auto shader = Engine::instance()->assetSystem->getContent<Shader>("deferred");
    fboShader.setShader(shader);
    auto io = fboShader.uniformIO();
    io.setParam<Tbo *>("uTexture", fbo->textures[0]);
    //io.setParam<Tbo *>("uTexture", &Engine::instance()->assetSystem->getContent<Image>("logo")->getTexture());
    fboMesh = new MeshBuffer(Engine::instance()->assetSystem->getContent<MeshData>("plane"));
}
void SceneOverlay::onDetach() {

}
void SceneOverlay::onRender(const double &deltaTime) {
	fbo->bind();
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
	fbo->unbind();
   
   
    if (views.size() == 0)
        return;
   fboMesh->bind();
   fboShader.bind();
   
   auto io = fboShader.uniformIO();   
   Transform pos;
   pos.position = Vector3(0, 0, 25);
   pos.rotator.rotate(90, 0, 0);
   io.setParam<glm::mat4>("uView", views.front()->transform->getMat());
   io.setParam<glm::mat4>("uProj", views.front()->viewMode.mProjection);
   io.setParam<glm::mat4>("uModel", pos.getMat());   
   fboShader.upload();
   
   fboMesh->draw();
   fboShader.unbind();
   fboMesh->unbind();
}