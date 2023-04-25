#pragma once
#ifndef _glbuffer
#define _glbuffer

#include "GLUtils.hpp"
#include "Math.hpp"
#include "Mesh.hpp"
#include <list>
using namespace std;

#pragma warning( disable : 4312 4267 4838)

struct Ibo {
	unsigned id = 0, size = 0;

	Ibo();
	~Ibo();
	void bind();
	void unbind();
	void setBuffer(void *data, unsigned dataSize);
};
struct Vbo {
	unsigned id;
	Vbo();
	~Vbo();
	void bind();
	void unbind();
};
struct Vao {
private:
	unsigned vao;
public:
	Vao();
	~Vao();
	void bind();
	void unbind();
};
struct Tbo {
	//https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glReadPixels.xhtml
protected:
	int width = 0, height = 0;
	Color *buffer = nullptr;
public:
	unsigned id = 0;
	Tbo();
	virtual ~Tbo();
	
	void loadToGPU();

	void bind();
	void unbind();

	void create(int width, int height, Color *imageBuffer);
	void destroy();

	void setPixel(unsigned x, unsigned y, Color color);
	Color getPixel(unsigned x, unsigned y);
	Color *&getBuffer() { return buffer; }
	bool isValid() const { return buffer; }
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

	list<EntityData> entities;
	unsigned stride = 0;
public:
	template<class T>VboBuilder &addAttrib(unsigned componentSize, bool glNormalized = false) {
		EntityData newAttribute = {
				entities.size(),
				componentSize,
				componentSize * sizeof(T),
				Conversors::typeCToGL<T>(),
				glNormalized
		};

		entities.push_back(newAttribute);
		stride += newAttribute.subBufferSize;
		return *this;
	}
	template<class T>VboBuilder &setBuffer(T *buffer, unsigned rows) {
		glDCall(glBufferData(GL_ARRAY_BUFFER, rows * sizeof(T), buffer, GL_STATIC_DRAW));
		return *this;
	}
	template<> VboBuilder &setBuffer(void *data, unsigned dataSize) {
		glDCall(glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW));
		return *this;
	}
	VboBuilder &addAttrib(unsigned attribID, unsigned componentSize, unsigned stride, unsigned int pointer = 0, unsigned type = GL_FLOAT, bool glNormalized = false);
	void build();
	Inline bool isValid() const { return stride != 0; }
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
#endif //!_glbuffer