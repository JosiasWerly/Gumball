
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
	auto &assetsSystem = *Engine::instance()->getSystem<AssetsSystem>();
	if (assetsSystem(name, meshData)) {

		/*struct SuperFoo {
			float x, y, z, w;
			float xn, yn, zn, wn;
			float xv, yv;
		};
		SuperFoo data[] = {
			{0.0, 0.0, 0.0, 1.0,		0.0, 0.0, 0.0, 1.0,				0, 0},
			{0.3, 0.0, 0.0, 1.0,		0.0, 0.0, 0.0, 1.0,				1, 0},
			{0.3, 0.3, 0.0, 1.0,		0.0, 0.0, 0.0, 1.0,				1, 1},
			{0.0, 0.3, 0.0, 1.0,		0.0, 0.0, 0.0, 1.0,				0, 1}
		};
		VboBuilder()
			.setBuffer<SuperFoo>(data, 4)
			.addAttrib<float>(4)
			.addAttrib<float>(4)
			.addAttrib<float>(2)
			.build();

		unsigned IndexBuffer[]{
			0, 1, 2,
			2, 3, 0
		};
		ibo->setBuffer(
			IndexBuffer,
			6 * sizeof(float));*/


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