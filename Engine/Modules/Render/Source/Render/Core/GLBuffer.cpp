#include "GLBuffer.hpp"
#include "Shaders.hpp"
#include "Texture.hpp"

#include "stbImage/stb_image.h"

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
void DrawableBuffer::bindAll() const {
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
	glDrawElements(GL_TRIANGLES, ibo->getSize(), GL_UNSIGNED_INT, nullptr);
}

Fbo::Fbo() {
	glGenFramebuffers(1, &id);
	//glRenderbufferStorage
}
Fbo::~Fbo() {
	glDeleteFramebuffers(1, &id);
	for (auto t : textures) {
		delete t;
	}
}
void Fbo::bind(Fbo::ETarget target) {
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
	Texture *newTexture = new Texture;
	newTexture->create(size, nullptr);
	newTexture->bind();
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + textures.size(), GL_TEXTURE_2D, newTexture->getId(), 0);
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
		glBindTexture(GL_TEXTURE_2D, textures[i]->getId());
	}
}

#pragma warning( default : 4312 4267 4838)


//unsigned vao;
//glGenVertexArrays(1, &vao);
//glBindVertexArray(vao);
//
//unsigned vbo;
//glGenBuffers(1, &vbo);
//glBindBuffer(GL_ARRAY_BUFFER, vbo);
//glBufferData(GL_ARRAY_BUFFER, sizeof(pos), pos, GL_STATIC_DRAW);
//
//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
//glEnableVertexAttribArray(0);
//
//unsigned ibo;
//glGenBuffers(1, &ibo);
//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(id), id, GL_STATIC_DRAW);