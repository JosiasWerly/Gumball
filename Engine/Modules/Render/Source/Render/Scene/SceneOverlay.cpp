#include "SceneOverlay.hpp"
#include "Core/GLBuffer.hpp"
#include "Render.module.hpp"

#include <Content/Content.module.hpp>


ViewHandle::ViewMode::ViewMode() {
	setProjectionPerspective();
}
void ViewHandle::ViewMode::setProjectionPerspective(float fovy, float aspect, float zNear, float zFar) {
	mProjection = glm::perspective(fovy, aspect, zNear, zFar);
	eProjection = eProjectionMode::Perspective;
}
void ViewHandle::ViewMode::setProjectionOrtho(float left, float right, float bottom, float top, float zNear, float zFar) {
	mProjection = glm::ortho(left, right, bottom, top, zNear, zFar);
	eProjection = eProjectionMode::Ortho;
}
void ViewHandle::ViewMode::setProjectionCustom(glm::mat4 customProj) {
	mProjection = customProj;
	eProjection = eProjectionMode::Custom;
}
const glm::mat4 &ViewHandle::ViewMode::getProjection() {
	return mProjection;
}

ViewHandle::ViewHandle() {
}
ViewHandle::~ViewHandle() {
	disable();
}
void ViewHandle::enable() {
	RenderModule::instance()->getSceneOverlay().views.push_back(this);
}
void ViewHandle::disable() {
	RenderModule::instance()->getSceneOverlay().views.remove(this);
}

DrawHandle::DrawHandle() {
	static Shader *geometry = ContentModule::instance()->getContent<Shader>("geometry");
	setShader(geometry);
}
DrawHandle::~DrawHandle() {
	disable();
}
void DrawHandle::enable() {
	RenderModule::instance()->getSceneOverlay().draws.push_back(this);
}
void DrawHandle::disable() {
	RenderModule::instance()->getSceneOverlay().draws.remove(this);
}
void DrawHandle::setMesh(MeshData *newMesh) {
	meshInstance.setMeshData(newMesh);
}
void DrawHandle::setShader(Shader *newShader) {
	shaderInstance.setShader(newShader);
}

FboHandle::FboHandle() {
	screenMesh.setMeshData(ContentModule::instance()->getContent<MeshData>("screenPlane"));
	fbo = new Fbo;
}
FboHandle::~FboHandle() {
	delete fbo;
}
void FboHandle::setShader(Shader *newShader) {
	shaderInstance.setShader(newShader);
	fbo->bind();
	fbo->clearTextures();
	auto &uniforms = shaderInstance.getShader()->getUniforms();

	for (auto it : uniforms.getUniforms()) {
		uniforms.setActive(it->info.name, true);
		if (EUniformType::utexture == it->info.type) {
			fbo->addTexture({ 800, 600 });
			uniforms.set<Texture *>(it->info.name, fbo->getTextures().back());
		}
	}
	fbo->updateDrawBuffers();
	fbo->unbind();
}
void FboHandle::flush() {
	fbo->bind(Fbo::ETarget::Write);
	fbo->clearBuffer();
}
void FboHandle::render() {
	fbo->unbind();
	fbo->bind(Fbo::ETarget::Read);
	Shader *shader = shaderInstance.getShader();
	shader->bind();
	shader->upload();
	screenMesh.bind();
	screenMesh.draw();
	fbo->unbind();
	shader->unbind();
}


//static ViewHandle *hView;
//static DrawHandle *hDraw;
void SceneOverlay::onAttach() {
	gbuffer = new FboHandle;
	gbuffer->setShader(ContentModule::instance()->getContent<Shader>("gbuffer"));

	geometryShader = ContentModule::instance()->getContent<Shader>("geometry");
	auto &params = geometryShader->getUniforms();
	params.activate({ "gAlbedo" });



	//hView = new ViewHandle;
	//hView->viewMode.setProjectionPerspective();
	//hView->enable();
	//hView->transform = new Transform;
	//hView->transform->position.z = -30;
	//
	//ContentModule *cm = ContentModule::instance();
	//hDraw = new DrawHandle;
	//auto &drawParam = hDraw->getShaderInstance().getParameters();
	//drawParam.activate({ "uTexture", "uColor" });
	//drawParam.set<Texture *>("uTexture", cm->getContent<Texture>("uv_grid"));
	//drawParam.set<Color>("uColor", 0xffffffff);
	//hDraw->setMesh(cm->getContent<MeshData>("cube"));
	//hDraw->enable();
	//hDraw->transform = new Transform;
	//hDraw->transform->scale = Vector3(0.001, 0.001, 0.001);
	//hDraw->transform->position = Vector3(10, 0, 0);
}
void SceneOverlay::onDetach() {

}
void SceneOverlay::onRender(const double &deltaTime) {
	/* render
	* 1. geometry: normal geometry shader.(output goes to gbuffer)
	* 2. custom geometry: custom shader.(output goes to gbuffer)
	* 3. lighting(gbuffer).
	* 5. postprocess
	*/
	//https://learnopengl.com/code_viewer_gh.php?code=src/5.advanced_lighting/8.2.deferred_shading_volumes/deferred_shading_volumes.cpp
	gbuffer->flush();

	ShaderUniforms &geometryUniforms = geometryShader->getUniforms();
	geometryShader->bind();
	geometryUniforms.uploadActive();
	for (auto &v : views) {
		const auto mView = v->transform->getMatrix();
		const auto mProjection = v->viewMode.mProjection;
		geometryUniforms.sync<glm::mat4>("uView", mView);
		geometryUniforms.sync<glm::mat4>("uProj", mProjection);

		for (auto &d : draws) {
			auto &mesh = d->meshInstance;
			geometryUniforms.sync<glm::mat4>("uModel", d->transform->getMatrix());
			d->shaderInstance.getParameters().uploadActive();
			mesh.bind();
			mesh.draw();
		}
	}
	geometryShader->unbind();

	gbuffer->render();
}