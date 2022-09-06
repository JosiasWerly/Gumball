
#include "Drawable.hpp"
#include "Engine.hpp"
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

bool DrawInstance::setMesh(string name) {
	bind();
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
	SVar<Object, Image> img = Engine::instance()->assetSystem->getAsset(name)->getContent();
	material.shader->getUniform<EUniformType::u_stexture>("uTexture")->image = img;
	return true;
}
