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