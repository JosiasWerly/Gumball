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
void Ibo::unbind() {
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
void Vbo::unbind() {
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
void Vao::unbind() {
	glDCall(glBindVertexArray(0));
}


//https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glReadPixels.xhtml
Tbo::Tbo() {

	//glGenTextures(1, &gPosition);
	//glBindTexture(GL_TEXTURE_2D, gPosition);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);

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
void Tbo::loadToGPU() {
	bind();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
	unbind();
}
void Tbo::bind() {
	glBindTexture(GL_TEXTURE_2D, id);
}
void Tbo::unbind() {
	glBindTexture(GL_TEXTURE_2D, 0);
}
void Tbo::create(int width, int height, Color *newBuffer) {
	this->width = width;
	this->height = height;
	this->buffer = newBuffer;

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}
void Tbo::destroy() {
	if (width > 0 || height > 0) {
		glDeleteTextures(1, &id);
		id = 0;
		width = 0;
		height = 0;
		delete[] buffer;
		buffer = nullptr;
	}
}
void Tbo::setPixel(unsigned x, unsigned y, Color color) {
	const int p = ((y * height) + x) * 4;
	if (p > width * height)
		return;
	buffer[p] = color;
}
Color Tbo::getPixel(unsigned x, unsigned y) {
	const int p = ((y * height) + x) * 4;
	return buffer[p];
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