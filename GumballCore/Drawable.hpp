#pragma once
#ifndef __drawable
#define __drawable

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <list>
#include "Utils.hpp"
using namespace std;


#pragma warning( disable : 4312)

struct Ibo {
	unsigned id, size;

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
struct VboLayout {
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
	template<class T>VboLayout &addAttrib(unsigned componentSize, bool glNormalized = false) {
		EntityData newAttribute = {
				Entities.size(),
				componentSize,
				componentSize * sizeof(T),
				Conversors::typeCToGL<T>(),
				glNormalized,

		};

		Entities.push_back(newAttribute);
		stride += newAttribute.subBufferSize;
		return *this;
	}
	template<class T>VboLayout &setBuffer(T *buffer, unsigned rows) {
		glBufferData(GL_ARRAY_BUFFER, rows * sizeof(T), buffer, GL_STATIC_DRAW);
		return *this;
	}

	VboLayout &setBuffer(void *data, unsigned dataSize) {
		glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);
		return *this;
	}
	VboLayout &addAttrib(unsigned attribID, unsigned componentSize, unsigned stride, unsigned int pointer = 0, unsigned type = GL_FLOAT, bool glNormalized = false) {

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

class DrawInstance{
public:
	Vao vao;
	Vbo vbo;
	Ibo ibo;

	DrawInstance() {
		vao.bind();
		vbo.bind();

		struct SuperFoo {
			float x, y, z;
			float displace;
		};
		SuperFoo data[] = {
			{0, 0, 0,			0.0},
			{0.1, 0.0, 0,		0.1},
			{0.1, 0.1, 0.0,		0.2},
			{0, 0.1, 0.0,		0.3}
		};
		VboLayout()
			.setBuffer<SuperFoo>(data, 4)
			.addAttrib<float>(3)
			.addAttrib<float>(1)
			.build();


		ibo.bind();
		ibo.setBuffer(
			new unsigned[5]{
				0, 1, 2,
				2, 3
			},
			5 * sizeof(float));
		vao.unBind();
		vbo.unbind();
		ibo.unbind();
	}

	inline void draw() {
		vao.bind();
		glDrawElements(GL_TRIANGLES, ibo.size, GL_UNSIGNED_INT, nullptr);
	}
};

#endif