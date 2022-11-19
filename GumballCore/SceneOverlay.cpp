#include "SceneOverlay.hpp"
#include "GLBuffer.hpp"


View::View() {
    static auto scene = Engine::instance()->renderSystem->scene;
    scene->views.push_back(this);
}
View::~View() {
    static auto scene = Engine::instance()->renderSystem->scene;
    scene->views.remove(this);
}

DrawCallData::DrawCallData(MeshData *mesh) {
    vao = new Vao();
    vao->bind();
    vbo = new Vbo();
    vbo->bind();
    ibo = new Ibo();
    ibo->bind();
    meshData = mesh;

    VboBuilder()
        .setBuffer<void>(meshData->mesh.data(), (unsigned)meshData->mesh.size() * sizeof(MeshVertexData))
        .addAttrib<float>(3)//pos
        .addAttrib<float>(3)//normal
        .addAttrib<float>(2)//uv
        .build();

    ibo->setBuffer(
        meshData->index.data(),
        static_cast<unsigned int> (meshData->index.size() * sizeof(unsigned)));
    unBind();

}
DrawCallData::~DrawCallData() {
    vao->unBind();
    vbo->unBind();
    ibo->unBind();
    delete vao;
    delete vbo;
    delete ibo;
}
Inline void DrawCallData::bind() {
    vao->bind();
}
Inline void DrawCallData::unBind() {
    vao->unBind();
}
Inline void DrawCallData::draw() {
    glDrawElements(GL_TRIANGLES, ibo->size, GL_UNSIGNED_INT, nullptr);
}

DrawCallInstance::DrawCallInstance(string meshName){
    static auto scene = Engine::instance()->renderSystem->scene;
    MeshData *mData = Engine::instance()->assetSystem->getAsset(meshName)->getContent().pin<MeshData>();
    if (!mData)
        throw 1;
    auto drawCallLayer = scene->findDrawLayer(mData);
    drawData = drawCallLayer->data;
    drawCallLayer->instances.push_back(this);
}
DrawCallInstance::~DrawCallInstance() {
    static auto scene = Engine::instance()->renderSystem->scene;
    auto drawCallLayer = scene->findDrawLayer(drawData->getMeshData());
    drawCallLayer->instances.remove(this);
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
	for (auto &view : views) {
		auto viewMat = view->transform.getMat();
        for (auto &drawLayer : drawCallLayers) {
            drawLayer.data->bind();
            for (auto &drawCall : drawLayer.instances) {
                drawCall->material.bind();
                drawCall->material.param<EUniformType::u_mat>("uView")->value = viewMat;
                drawCall->material.param<EUniformType::u_mat>("uProj")->value = view->viewMode.mProjection;
                drawCall->material.param<EUniformType::u_mat>("uModel")->value = drawCall->transform.getMat();
                drawCall->material.uploadParams();
                drawLayer.data->draw();
            }
        }
	}
}

//DrawInstance::DrawInstance() {
//	vao = new Vao;
//	vao->bind();
//	vbo = new Vbo;
//	vbo->bind();
//	ibo = new Ibo;
//	ibo->bind();
//
//	vao->unBind();
//	vbo->unbind();
//	ibo->unbind();
//
//	Shader *sh = Engine::instance()->assetSystem->getAsset("default")->getContent().pin<Shader>();
//	material.setShader(sh);
//	material.param<EUniformType::u_fvec4>("uColor")->value = glm::vec4(1, 1, 1, 0);
//	material.param<EUniformType::u_stexture>("uTexture")->image = Engine::instance()->assetSystem->getAsset("logo")->getContent().pin<Image>();
//}
//DrawInstance::~DrawInstance() {
//}
//bool DrawInstance::setMesh(string name) {
//	vao->bind();
//	vbo->bind();
//	ibo->bind();
//	meshData = Engine::instance()->assetSystem->getAsset(name)->getContent().pin<MeshData>();
//	if (meshData) {
//		VboBuilder()
//			.setBuffer<void>(meshData->mesh.data(), (unsigned)meshData->mesh.size() * sizeof(MeshVertexData))
//			.addAttrib<float>(3)//pos
//			.addAttrib<float>(3)//normal
//			.addAttrib<float>(2)//uv
//			.build();
//
//		ibo->setBuffer(
//			meshData->index.data(),
//			static_cast<unsigned int> (meshData->index.size() * sizeof(unsigned)));
//		unbind();
//		return true;
//	}
//	return false;
//}
//bool DrawInstance::setTexture(string name) {
//	Image *img = Engine::instance()->assetSystem->getAsset(name)->getContent().pin<Image>();
//	material.param<EUniformType::u_stexture>("uTexture")->image = img;
//	return true;
//}
//void DrawInstance::bind() {
//	material.use();
//	vao->bind();
//	vbo->bind();
//	ibo->bind();
//}
//void DrawInstance::unbind() {
//	vao->unBind();
//	vbo->unbind();
//	ibo->unbind();
//}
//void DrawInstance::draw() {
//	material.use();
//	vao->bind();
//	glDrawElements(GL_TRIANGLES, ibo->size, GL_UNSIGNED_INT, nullptr);
//}