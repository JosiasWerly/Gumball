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
    meshInstance.setMeshData(newMesh);
}
void DrawHandle::setShader(Shader *newShader) {
    auto as = Engine::instance()->assetSystem;
    shaderInstance.setShader(newShader);
    if (newShader) {
        shaderInstance.bind();
        auto &io = shaderInstance.uniformIO();        
        io.setParam<Color>("uColor", 0xffffffff);
        io.upload();
        shaderInstance.unbind();
    }
}

FboHandle::FboHandle() {
    auto &as = Engine::instance()->assetSystem;
    screenMesh.setMeshData(as->getContent<MeshData>("screenPlane"));
}
FboHandle::~FboHandle() {
}
void FboHandle::setShader(Shader *newShader) {
    shaderInstance.setShader(newShader);
    fbo.bind();
    fbo.clearTextures();
    auto uniforms = shaderInstance.getShader()->getUniforms();
	auto io = shaderInstance.uniformIO();

	for (auto it : uniforms) {
		if (EUniformType::u_stexture == it.type) {
			fbo.addTexture({ 800, 600 });
			io.setParam<Tbo *>(it.name, fbo.textures.back());
		}
	}
    fbo.updateDrawBuffers();
    fbo.unbind();
}
void FboHandle::render() {
    fbo.unbind();
    fbo.bind(EFboTarget::Read);
    screenMesh.bind();
    shaderInstance.bind();
    shaderInstance.upload();
    screenMesh.draw();
    fbo.unbind();
}



SceneOverlay::SceneOverlay() : 
	IRenderOverlay("scene") {
}
SceneOverlay::~SceneOverlay() {
}
void SceneOverlay::onAttach() {
    auto &as = Engine::instance()->assetSystem;
    Gbuffer = new FboHandle;
    Gbuffer->setShader(as->getContent<Shader>("gbuffer"));
}
void SceneOverlay::onDetach() {

}
void SceneOverlay::onRender(const double &deltaTime) {
    auto &fbo = Gbuffer->getFbo();    
    fbo.bind(EFboTarget::Write);
    fbo.clearBuffer();
    for (auto &v : views) {
    	const auto mView = v->transform->getMat();
        const auto mProjection = v->viewMode.mProjection;
    	for (auto &d : draws) {
    		auto &mesh = d->meshInstance;
    		auto &shader = d->shaderInstance;
    		auto &shaderIO = shader.uniformIO();
    		mesh.bind();
    		shader.bind();
    		shaderIO.setParam<glm::mat4>("uView", mView);
    		shaderIO.setParam<glm::mat4>("uProj", mProjection);
    		shaderIO.setParam<glm::mat4>("uModel", d->transform->getMat());
    		shaderIO.upload();
    		mesh.draw();
    		shader.unbind();
    	}
    }
    Gbuffer->render();    
}