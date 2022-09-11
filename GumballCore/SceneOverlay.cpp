#include "SceneOverlay.hpp"
#include "GLBuffer.hpp"

SceneOverlay::SceneOverlay() : 
	IRenderOverlay("scene") {
}
SceneOverlay::~SceneOverlay() {
}
void SceneOverlay::onAttach() {

}
void SceneOverlay::onDetach() {

}
void SceneOverlay::onRender(float deltaTime) {
	for (auto &view : views) {
		auto viewMat = view->transform.getMat();
		for (auto &draw : drawInstances) {
			draw->material.param<EUniformType::u_mat>("uView")->value = viewMat;
			draw->material.param<EUniformType::u_mat>("uProj")->value = view->viewMode.mProjection;
			draw->material.param<EUniformType::u_mat>("uModel")->value = draw->transform.getMat();
			draw->draw();
		}
	}
}
void SceneOverlay::pushView(View *view) {
	views.push_back(view);
}
void SceneOverlay::popView(View *view) {
	views.remove(view);
}
void SceneOverlay::pushDrawInstance(DrawInstance *instance, bool front) {
	if (front)
		drawInstances.push_front(instance);
	else
		drawInstances.push_back(instance);
}
void SceneOverlay::popDrawInstance(DrawInstance *drawInstance) {
	drawInstances.remove(drawInstance);
}

DrawInstance::DrawInstance() {
	vao = new Vao;
	vao->bind();
	vbo = new Vbo;
	vbo->bind();
	ibo = new Ibo;
	ibo->bind();

	vao->unBind();
	vbo->unbind();
	ibo->unbind();

	Shader *sh = Engine::instance()->assetSystem->getAsset("default")->getContent().pin<Shader>();
	material.setShader(sh);
	material.param<EUniformType::u_fvec4>("uColor")->value = glm::vec4(1, 1, 1, 0);
	material.param<EUniformType::u_stexture>("uTexture")->image = Engine::instance()->assetSystem->getAsset("logo")->getContent().pin<Image>();
}
DrawInstance::~DrawInstance() {
}
bool DrawInstance::setMesh(string name) {
	vao->bind();
	vbo->bind();
	ibo->bind();
	MeshData *meshData = Engine::instance()->assetSystem->getAsset(name)->getContent().pin<MeshData>();
	if (meshData) {
		VboBuilder()
			.setBuffer<void>(meshData->mesh.data(), (unsigned)meshData->mesh.size() * sizeof(MeshVertexData))
			.addAttrib<float>(3)//pos
			.addAttrib<float>(3)//normal
			.addAttrib<float>(2)//uv
			.build();

		ibo->setBuffer(
			meshData->index.data(),
			static_cast<unsigned int> (meshData->index.size() * sizeof(unsigned)));
		unbind();
		return true;
	}
	return false;
}
bool DrawInstance::setTexture(string name) {
	Image *img = Engine::instance()->assetSystem->getAsset(name)->getContent().pin<Image>();
	material.param<EUniformType::u_stexture>("uTexture")->image = img;
	return true;
}
void DrawInstance::bind() {
	material.use();
	vao->bind();
	vbo->bind();
	ibo->bind();
}
void DrawInstance::unbind() {
	vao->unBind();
	vbo->unbind();
	ibo->unbind();
}
void DrawInstance::draw() {
	material.use();
	vao->bind();
	glDrawElements(GL_TRIANGLES, ibo->size, GL_UNSIGNED_INT, nullptr);
}