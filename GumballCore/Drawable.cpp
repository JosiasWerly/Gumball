
#include "Drawable.hpp"
#include "Engine.hpp"
#include "Shaders.hpp"
#include "stb_image/stb_image.h"

#include <iostream>

void Tbo::loadTexture(string path) {
	bind();
	if (imageBuffer) {
		stbi_image_free(imageBuffer);
		imageBuffer = nullptr;
	}	
	
	int ch = 0;
	stbi_set_flip_vertically_on_load(true);
	if (imageBuffer = stbi_load(path.c_str(), &width, &height, &ch, 4)) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageBuffer);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	unbind();
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


	;
	material.shader.assign(static_cast<Shader *>(Engine::instance()->assetSystem->getAsset("default")->getContentCloned()));
	material.shader->getUniform<EUniformType::u_fvec4>("uColor")->value = glm::vec4(1, 1, 1, 0);
	material.shader->getUniform<EUniformType::u_stexture>("uTexture")->image = Engine::instance()->assetSystem->getAsset("logo")->getContent().pin<Image>();
}
bool DrawInstance::setMesh(string name) {
	vao->bind();
	vbo->bind();
	ibo->bind();
	MeshData* meshData = Engine::instance()->assetSystem->getAsset(name)->getContent().pin<MeshData>();
	if (meshData) {
		VboBuilder()
			.setBuffer<void>(meshData->mesh.data(), (unsigned)meshData->mesh.size() * sizeof(MeshVertexData))
			.addAttrib<float>(3)//pos
			.addAttrib<float>(3)//normal
			.addAttrib<float>(2)//uv
			.build();

		ibo->setBuffer(
			meshData->index.data(),
			meshData->index.size() * sizeof(unsigned));
		unbind();
		return true;
	}
	return false;
}
bool DrawInstance::setTexture(string name) {
	Image *img = Engine::instance()->assetSystem->getAsset(name)->getContent().pin<Image>();
	material.shader->getUniform<EUniformType::u_stexture>("uTexture")->image = img;
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
