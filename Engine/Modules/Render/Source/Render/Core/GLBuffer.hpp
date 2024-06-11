#pragma once
#ifndef _glbuffer
#define _glbuffer

#include "GLUtils.hpp"

#include <list>
#include <vector>
using namespace std;

#pragma warning( disable : 4312 4267 4838)

struct Ibo {
private:
	unsigned id = 0, size = 0;

public:
	Ibo();
	~Ibo();
	void bind();
	void unbind();
	void setBuffer(void *data, unsigned dataSize);

	const unsigned &getId() { return id; }
	const unsigned &getSize() { return size; }
};

struct Vbo {
private:
	unsigned id;

public:
	Vbo();
	~Vbo();
	void bind();
	void unbind();
	const unsigned &getId() { return id; }
};

struct Vao {
private:
	unsigned vao; //todo: change to ID

public:
	Vao();
	~Vao();
	void bind();
	void unbind();
	const unsigned &getId() { return vao; }
};

//I should make the class VBO contains a method that returns this struct, drawable properly the builder pattern.
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

class DrawableBuffer {
	Vao *vao = nullptr;
	Vbo *vbo = nullptr;
	Ibo *ibo = nullptr;

public:
	DrawableBuffer();
	~DrawableBuffer();
	void bind() const;
	void unbind() const;
	void bindAll() const;
	void unbindAll() const;
	void draw() const;

	Inline Vao &getVao() const { return *vao; }
	Inline Vbo &getVbo() const { return *vbo; }
	Inline Ibo &getIbo() const { return *ibo; }
};

struct Fbo {
public:
	enum class ETarget {
		Read = GL_READ_FRAMEBUFFER,
		Write = GL_DRAW_FRAMEBUFFER,
		ReadWrite = GL_FRAMEBUFFER
	};

private:
	ETarget trg = ETarget::ReadWrite;
	unsigned id = 0;
	vector<class Texture *> textures;

public:
	Fbo();
	~Fbo();
	void bind(ETarget target = ETarget::ReadWrite);
	void unbind();
	void updateDrawBuffers();
	void clearBuffer();
	void beginDraw();
	
	void addTexture(Vector2i size);
	void clearTextures();
	vector<class Texture *> &getTextures() { return textures; }

	bool isValid();
	//glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
};

#pragma warning( default : 4312 4267 4838)
#endif //!_glbuffer