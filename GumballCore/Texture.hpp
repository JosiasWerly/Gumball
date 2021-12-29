#pragma once
#ifndef __texture
#define __texture
#include "AssetManager.hpp"
#include "GLUtils.hpp"

#include <iostream>
#include "stb_image/stb_image.h"
using namespace std;



struct TextureData {
	int width = 0, height = 0;
	unsigned char *memoryBuffer = nullptr;
};

class Texture {
protected:
	char slot = 0;
	int width = 0, height = 0;
	unsigned char *memoryBuffer = nullptr;
public:
	unsigned id = 0;
	Texture() {
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	virtual ~Texture() {
		glDeleteTextures(1, &id);
	}
	void setPixel(int x, int y, int color) {
		int p = ((y * height) + x) * 4;
		if (!memoryBuffer || p > width * height)
			return;
		memoryBuffer[p] = (color >> 24);
		memoryBuffer[p + 1] = (color >> 16);
		memoryBuffer[p + 2] = (color >> 8);
		memoryBuffer[p + 3] = color;
	}

	bool loadTexture(Archive &ar);
	void uploadBuffer() {
		this->bind();
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, memoryBuffer);
		glBindTexture(GL_TEXTURE_2D, id);
	}
	void downloadBuffer() {
		throw 1;
	}
	void bind() {
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, id);
	}
	void unbind() {
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	inline bool isValid() { return memoryBuffer; }
	inline void setSlot(int newSlot) { slot = newSlot; }
	inline int getSlot() { return slot; }
};



class TextureFactory : 
	public IAssetFactory {
public:
	TextureFactory() :
		IAssetFactory("TextureFactory") {
		this->extensions = { "png" };
	}
	virtual bool assemble(Asset &asset, Archive &ar) {
		Texture *newTexture = new Texture;
		if (newTexture->loadTexture(ar)) {
			asset << newTexture;
			return true;
		}
		return false;
	}
	virtual bool disassemble(Asset &asset, Archive &ar) {
		return true;
	}
};


#endif // !__shaders




//#shader vertex
//#version 330 core
//in vec4 pos;
//in vec2 texCoord;
//out vec2 vTexture;
//
////uniform mat4 uProj, uView, uModel;
//uniform mat4 uProj;
//uniform mat4 uView;
//uniform mat4 uModel;
//void main() {
//	vTexture = texCoord;
//	gl_Position = uProj * uView * uModel * pos;
//};
//
//
//#shader fragment
//#version 330 core
//
//in vec2 vTexture;
//out vec4 color;
//
//uniform vec4 uColor;
//uniform sampler2D uTexture;
//
//void main() {
//	vec4 texColor = texture(uTexture, vTexture);
//	color = texColor * uColor;
//};