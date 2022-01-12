#pragma once
#ifndef __drawable
#define __drawable

#include "GLUtils.hpp"
#include "Shaders.hpp"
#include "Texture.hpp"
#include <list>
using namespace std;


#pragma warning( disable : 4312 4267 4838)

struct Ibo {
	unsigned id = 0, size = 0;

	Ibo() {
		glGenBuffers(1, &id);
	}
	~Ibo() {
		glDeleteBuffers(1, &id);
	}
	inline void bind() {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
	}
	inline void unbind() {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	inline void setBuffer(void *data, unsigned dataSize) {
		size = dataSize;
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);
	}
};
struct Vbo {
	unsigned id;
	Vbo() {
		glGenBuffers(1, &id);
	}
	~Vbo() {
		glDeleteBuffers(1, &id);
	}
	inline void bind() {
		glBindBuffer(GL_ARRAY_BUFFER, id);
	}
	inline void unbind() {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
};
struct Vao {
private:
	unsigned vao;
public:
	Vao() {
		glGenVertexArrays(1, &vao);
	}
	~Vao() {
		glDeleteVertexArrays(1, &vao);
	}
	inline void bind() {
		glBindVertexArray(vao);
	}
	inline void unBind() {
		glBindVertexArray(0);
	}
};
struct Tbo {
	//https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glReadPixels.xhtml
protected:
	char slot = 0;
	int width = 0, height = 0;
	unsigned char *imageBuffer = nullptr;
public:
	unsigned id = 0;
	Tbo() {
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	virtual ~Tbo() {
		glDeleteTextures(1, &id);
	}
	void setPixel(int x, int y, int color) {
		int p = ((y * height) + x) * 4;
		if (!imageBuffer || p > width * height)
			return;
		imageBuffer[p] = (color >> 24);
		imageBuffer[p + 1] = (color >> 16);
		imageBuffer[p + 2] = (color >> 8);
		imageBuffer[p + 3] = color;
	}
	void loadTexture(string path);
	void loadToGPU() {
		this->bind();
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageBuffer);
		glBindTexture(GL_TEXTURE_2D, id);
	}
	void bind() {		
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, id);
	}
	void unbind() {
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	inline bool isValid() { return imageBuffer; }
	inline void setSlot(int newSlot) { slot = newSlot; }	
	inline int getSlot() { return slot; }
};
struct VboBuilder {
private:
	struct EntityData {
		unsigned
			attribID,
			componentSize,
			subBufferSize,
			glType,
			glNormalized;
	};

	list<EntityData> Entities;
	unsigned stride = 0;
public:
	template<class T>VboBuilder &addAttrib(unsigned componentSize, bool glNormalized = false) {
		EntityData newAttribute = {
				Entities.size(),
				componentSize,
				componentSize * sizeof(T),
				Conversors::typeCToGL<T>(),
				glNormalized
		};

		Entities.push_back(newAttribute);
		stride += newAttribute.subBufferSize;
		return *this;
	}
	template<class T>VboBuilder &setBuffer(T *buffer, unsigned rows) {
		glBufferData(GL_ARRAY_BUFFER, rows * sizeof(T), buffer, GL_STATIC_DRAW);
		return *this;
	}

	VboBuilder &setBuffer(void *data, unsigned dataSize) {
		glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);
		return *this;
	}
	VboBuilder &addAttrib(unsigned attribID, unsigned componentSize, unsigned stride, unsigned int pointer = 0, unsigned type = GL_FLOAT, bool glNormalized = false) {

		glVertexAttribPointer(attribID, componentSize, type, glNormalized, stride, (void *)pointer);
		glEnableVertexAttribArray(attribID);
		return *this;
	}

	void build() {
		unsigned pointer = 0;
		for (auto &e : Entities) {
			glVertexAttribPointer(e.attribID, e.componentSize, e.glType, e.glNormalized, stride, (void *)pointer);
			glEnableVertexAttribArray(e.attribID);
			pointer += e.subBufferSize;
		}
	}
};

class DrawInstance {
	Vao vao;
	Vbo vbo;
	Ibo ibo;

	Material material;
	Texture texture;
public:
	DrawInstance() {
		vao.bind();
		vbo.bind();
		ibo.bind();

		texture.setImage("logo");

		material.setShader("default");
		material.setParameter<int>("uTexture", 0);
		material.setParameter<glm::vec4>("uColor", glm::vec4(1, 1, 1, 0));

		struct SuperFoo {
			float x, y, z, w;
			float xv, yv;
		};
		SuperFoo data[] = {
			{0, 0, 0, 1,				0, 0},
			{0.5, 0.0, 0, 1,			1, 0},
			{0.5, 0.5, 0.0, 1,			1, 1},
			{0, 0.5, 0.0, 1,			0, 1}
		};
		VboBuilder()
			.setBuffer<SuperFoo>(data, 4)
			.addAttrib<float>(4)
			.addAttrib<float>(2)
			.build();

		unsigned IndexBuffer[5]{
			0, 1, 2,
			2, 3
		};
		ibo.setBuffer(
			IndexBuffer,
			5 * sizeof(float));


		vao.unBind();
		vbo.unbind();
		ibo.unbind();
	}

	inline void bind() {
		vao.bind();
		vbo.bind();
		ibo.bind();
	}
	inline void unbind() {
		vao.unBind();
		vbo.unbind();
		ibo.unbind();
	}
	inline void draw() {
		material.use();
		texture.bind();
		vao.bind();
		glDrawElements(GL_TRIANGLES, ibo.size, GL_UNSIGNED_INT, nullptr);
	}
};

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


#pragma warning( default : 4312 4267 4838)
#endif