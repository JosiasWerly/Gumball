#include "GLBuffer.hpp"
#include "Engine.hpp"
#include "Shaders.hpp"
#include "stb_image/stb_image.h"

#include <iostream>
#include <string>

#pragma warning( disable : 4312 4267 4838)



Ibo::Ibo() {
	glGenBuffers(1, &id);
}
Ibo::~Ibo() {
	glDeleteBuffers(1, &id);
}
void Ibo::bind() {
	glDCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id));
}
void Ibo::unBind() {
	glDCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
void Ibo::setBuffer(void *data, unsigned dataSize) {
	size = dataSize;
	glDCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW));
}


Vbo::Vbo() {
	glGenBuffers(1, &id);
}
Vbo::~Vbo() {
	glDeleteBuffers(1, &id);
}
void Vbo::bind() {
	glDCall(glBindBuffer(GL_ARRAY_BUFFER, id));
}
void Vbo::unBind() {
	glDCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}


Vao::Vao() {
	glGenVertexArrays(1, &vao);
}
Vao::~Vao() {
	glDeleteVertexArrays(1, &vao);
}
void Vao::bind() {
	glDCall(glBindVertexArray(vao));
}
void Vao::unBind() {
	glDCall(glBindVertexArray(0));
}


//https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glReadPixels.xhtml
Tbo::Tbo() {
	glDCall(glGenTextures(1, &id));
	glDCall(glBindTexture(GL_TEXTURE_2D, id));
	glDCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	glDCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	glDCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	glDCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	glDCall(glBindTexture(GL_TEXTURE_2D, 0));
}
Tbo::~Tbo() {
	glDeleteTextures(1, &id);
}
void Tbo::setPixel(int x, int y, int color) {
	int p = ((y * height) + x) * 4;
	if (!imageBuffer || p > width * height)
		return;
	imageBuffer[p] = (color >> 24);
	imageBuffer[p + 1] = (color >> 16);
	imageBuffer[p + 2] = (color >> 8);
	imageBuffer[p + 3] = color;
}
void Tbo::loadToGPU() {
	this->bind();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageBuffer);
	glBindTexture(GL_TEXTURE_2D, id);
}
void Tbo::bind() {
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, id);
}
void Tbo::unBind() {
	glBindTexture(GL_TEXTURE_2D, 0);
}
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
	unBind();
}


VboBuilder &VboBuilder::addAttrib(unsigned attribID, unsigned componentSize, unsigned stride, unsigned int pointer, unsigned type, bool glNormalized) {
	glDCall(glVertexAttribPointer(attribID, componentSize, type, glNormalized, stride, (void *)pointer));
	glDCall(glEnableVertexAttribArray(attribID));
	return *this;
}
void VboBuilder::build() {
	unsigned pointer = 0;
	for (auto &e : entities) {
		glDCall(glVertexAttribPointer(e.attribID, e.componentSize, e.glType, e.glNormalized, stride, (void *)pointer));
		glDCall(glEnableVertexAttribArray(e.attribID));
		pointer += e.subBufferSize;
	}
}

#pragma warning( default : 4312 4267 4838)