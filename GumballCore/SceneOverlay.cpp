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
    auto &uniforms = shaderInstance.getShader()->getUniforms();

	for (auto it : uniforms.getUniforms()) {
		if (EUniformType::u_stexture == it->owner.type) {
			fbo.addTexture({ 800, 600 });
            uniforms.set<Tbo *>(it->owner.name, fbo.textures.back());
		}
	}
    fbo.updateDrawBuffers();
    fbo.unbind();
}
void FboHandle::flush() {
    fbo.bind(EFboTarget::Write);
    fbo.clearBuffer();
}
void FboHandle::render() {
    fbo.unbind();
    fbo.bind(EFboTarget::Read);
    screenMesh.bind();
    Shader *shader = shaderInstance.getShader();
    shader->bind();
    shader->upload();
    shader->unbind();
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
    gbuffer = new FboHandle;
    gbuffer->setShader(as->getContent<Shader>("gbuffer"));
    gbuffer->getFbo().unbind();

    geometryShader = as->getContent<Shader>("geometry");
    auto &params = geometryShader->getUniforms();
    params.set<Color>("uColor", 0xffffffff);
    params.set<Tbo *>("uTexture", &as->getContent<Image>("color_grid")->getTexture());
    params.setActive("uColor", true);
    params.setActive("uTexture", true);

}
void SceneOverlay::onDetach() {

}
void SceneOverlay::onRender(const double &deltaTime) {
    ShaderUniforms &geometryUniforms = geometryShader->getUniforms();
    geometryShader->bind();
    
    geometryUniforms.uploadActive();
    //gbuffer->flush();
    for (auto &v : views) {
        const auto mView = v->transform->getMat();
        const auto mProjection = v->viewMode.mProjection;
        geometryUniforms.sync<glm::mat4>("uView", mView);
        geometryUniforms.sync<glm::mat4>("uProj", mProjection);

        for (auto &d : draws) {
            auto &mesh = d->meshInstance;
            geometryUniforms.sync<glm::mat4>("uModel", d->transform->getMat());
            d->shaderInstance.getParameters().uploadActive();
            mesh.bind();
            mesh.draw();
        }

    }
    geometryShader->unbind();
    //gbuffer->render();

	/*gbuffer->flush();

	shaderInstance->bind();
	ShaderUniformIOBus &shaderIO = shaderInstance->uniformIO();

	for (auto &v : views) {
		const auto mView = v->transform->getMat();
		const auto mProjection = v->viewMode.mProjection;

		shaderIO.setParamInline<glm::mat4>("uView", mView);
		shaderIO.setParamInline<glm::mat4>("uProj", mProjection);
		shaderIO.uploadTextures();

		shaderIO.setParamInline<glm::mat4>("uModel", transforms[0].getMat());
		meshInstance[0]->bind();
		meshInstance[0]->draw();
		meshInstance[0]->unbind();

		shaderIO.setParamInline<glm::mat4>("uModel", transforms[1].getMat());
		meshInstance[1]->bind();
		meshInstance[1]->draw();
		meshInstance[1]->unbind();
	}
	shaderInstance->unbind();
	gbuffer->render();*/


    //auto &fbo = gbuffer->getFbo();
    //fbo.bind(EFboTarget::Write);
    //fbo.clearBuffer();
    //for (auto &v : views) {
    //	const auto mView = v->transform->getMat();
    //    const auto mProjection = v->viewMode.mProjection;
    //	for (auto &d : draws) {
    //		auto &mesh = d->meshInstance;
    //		auto &shader = d->shaderInstance;
    //		auto &shaderIO = shader.uniformIO();
    //		mesh.bind();
    //		shader.bind();
    //		shaderIO.setParam<glm::mat4>("uView", mView);
    //		shaderIO.setParam<glm::mat4>("uProj", mProjection);
    //		shaderIO.setParam<glm::mat4>("uModel", d->transform->getMat());
    //		shaderIO.upload();
    //		mesh.draw();
    //		shader.unbind();
    //	}
    //}
    //gbuffer->render();
}