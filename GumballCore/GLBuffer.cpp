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
}
Tbo::~Tbo() {
	destroy();
}
void Tbo::upload() {
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
void Tbo::create(int width, int height, unsigned internalFormat, unsigned format, unsigned type) {
	destroy();
	this->width = width;
	this->height = height;
	this->internalFormat = internalFormat;
	this->type = type;
	this->format = format;
	glDCall(glGenTextures(1, &id));
	bind();
	glTexStorage2D(GL_TEXTURE_2D, 1, internalFormat, width, height);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	unbind();
}
void Tbo::destroy() {
	if (id > 0 && width > 0 || height > 0) {
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
void Tbo::setBuffer(Color *newBuffer) {
	buffer = newBuffer;
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, format, type, buffer);
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

DrawableBuffer::DrawableBuffer() {
	vao = new Vao();
	vao->bind();
	vbo = new Vbo();
	vbo->bind();
	ibo = new Ibo();
	ibo->bind();

	unbindAll();
}
DrawableBuffer::~DrawableBuffer() {
	unbindAll();
	delete vao;
	delete vbo;
	delete ibo;
}
void DrawableBuffer::bind() const {
	vao->bind();
}
void DrawableBuffer::unbind() const {
	vao->unbind();
}
void DrawableBuffer::bindAll() const{
	vao->bind();
	vbo->bind();
	ibo->bind();
}
void DrawableBuffer::unbindAll() const {
	vao->unbind();
	vbo->unbind();
	ibo->unbind();
}
void DrawableBuffer::draw() const {
	glDrawElements(GL_TRIANGLES, ibo->size, GL_UNSIGNED_INT, nullptr);
}

Fbo::Fbo() {
	glGenFramebuffers(1, &id);
	for (auto t : textures) {
		delete t;
	}
}
Fbo::~Fbo() {
	glDeleteFramebuffers(1, &id);
}
void Fbo::bind(EFboTarget target) {
	trg = target;
	glBindFramebuffer(static_cast<unsigned>(trg), id);
}
void Fbo::unbind() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void Fbo::clearTextures() {
	for (int i = 0; i < textures.size(); ++i) {
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, 0, 0);
		delete textures[i];
	}
	textures.clear();
}
void Fbo::addTexture(Vector2i size) {
	Tbo *newTexture = new Tbo;
	newTexture->bind();
	newTexture->create(size.x, size.y);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + textures.size(), GL_TEXTURE_2D, newTexture->id, 0);
	textures.push_back(newTexture);
	newTexture->unbind();
}
void Fbo::updateDrawBuffers() {
	const int tNum = textures.size();
	unsigned *textureRelay = new unsigned[tNum];
	for (int i = 0; i < tNum; ++i) {
		textureRelay[i] = GL_COLOR_ATTACHMENT0 + i;
	}
	glDrawBuffers(tNum, textureRelay);
	delete[] textureRelay;
}
void Fbo::clearBuffer() {
	glClearColor(0, 0, 0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void Fbo::beginDraw() {
	for (int i = 0; i < textures.size(); ++i) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, textures[i]->id);
	}
}

#pragma warning( default : 4312 4267 4838)