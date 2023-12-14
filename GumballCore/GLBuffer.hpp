#pragma once
#ifndef _glbuffer
#define _glbuffer

#include "GLUtils.hpp"
#include "Math.hpp"
#include <list>
#include <vector>
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
//https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glReadPixels.xhtml
struct Tbo {
protected:
	int width = 0, height = 0;
	unsigned internalFormat, type, format;		
	Color *buffer = nullptr;
public:
	unsigned id = 0;
	Tbo();
	virtual ~Tbo();	
	void upload();
	void create(int width, int height, unsigned internalFormat = GL_RGBA8, unsigned format = GL_RGBA, unsigned type = GL_UNSIGNED_BYTE);
	void create(int width, int height, Color *buffer, unsigned internalFormat = GL_RGBA8, unsigned format = GL_RGBA, unsigned type = GL_UNSIGNED_BYTE);
	void destroy();
	void setPixel(unsigned x, unsigned y, Color color);
	Color getPixel(unsigned x, unsigned y);

	void setBuffer(Color *newBuffer);
	Color *&getBuffer() { return buffer; }
	
	void bind() const;
	void unbind();
	
	bool isValid() const { return buffer; }
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

enum class EFboTarget {
	Read = GL_READ_FRAMEBUFFER,
	Write = GL_DRAW_FRAMEBUFFER, 
	ReadWrite = GL_FRAMEBUFFER
};
struct Fbo {
private:
	EFboTarget trg = EFboTarget::ReadWrite;
	unsigned id = 0;

public:
	vector<Tbo *> textures;
	Fbo();
	~Fbo();
	void bind(EFboTarget target = EFboTarget::ReadWrite);
	void unbind();
	void clearTextures();
	void addTexture(Vector2i size);
	void updateDrawBuffers();
	void clearBuffer();
	void beginDraw();
	bool isCompleted();
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