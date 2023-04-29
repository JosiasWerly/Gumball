#pragma once
#ifndef __texture
#define __texture
#include "AssetManager.hpp"
#include "GLUtils.hpp"
#include "GLBuffer.hpp"
#include "Math.hpp"

#include <iostream>
#include "stb_image/stb_image.h"
using namespace std;


//TODO: change name to Texture
class Image :
	public Object {

	Tbo textureBuffer;
public:


	~Image() {
		destroy();
	}
	void create(int width, int height, Color *imageBuffer) {
		textureBuffer.bind();
		textureBuffer.create(width, height);
		textureBuffer.setBuffer(imageBuffer);
		textureBuffer.unbind();
	}
	void destroy() {
		//textureBuffer.destroy();
	}
	Inline void bind() {
		textureBuffer.bind();
	}
	Inline void unbind() {
		textureBuffer.unbind();
	}
	Inline void uploadBuffer() {
		textureBuffer.loadToGPU();
	}
	Inline bool isValid() {
		return textureBuffer.isValid();
	}

	virtual Var<Object> clone() const override { return Var<Object>(new Image); }
	virtual bool archiveLoad(Archive &ar) override;
	virtual bool archiveSave(Archive &ar) override;
};
#endif // !__texture



//class Texture {
//protected:
//	char slot = 0;
//	int width = 0, height = 0;
//	unsigned char *memoryBuffer = nullptr;
//public:
//	unsigned id = 0;
//	Texture() {
//		glGenTextures(1, &id);
//		glBindTexture(GL_TEXTURE_2D, id);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//		glBindTexture(GL_TEXTURE_2D, 0);
//	}
//	virtual ~Texture() {
//		glDeleteTextures(1, &id);
//	}
//	void setPixel(int x, int y, int color) {
//		int p = ((y * height) + x) * 4;
//		if (!memoryBuffer || p > width * height)
//			return;
//		memoryBuffer[p] = (color >> 24);
//		memoryBuffer[p + 1] = (color >> 16);
//		memoryBuffer[p + 2] = (color >> 8);
//		memoryBuffer[p + 3] = color;
//	}
//
//	bool loadTexture(Archive &ar);
//	void uploadBuffer() {
//		this->bind();
//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, memoryBuffer);
//		glBindTexture(GL_TEXTURE_2D, id);
//	}
//	void downloadBuffer() {
//		throw 1;
//	}
//	void bind() {
//		glActiveTexture(GL_TEXTURE0 + slot);
//		glBindTexture(GL_TEXTURE_2D, id);
//	}
//	void unbind() {
//		glBindTexture(GL_TEXTURE_2D, 0);
//	}
//	inline bool isValid() { return memoryBuffer; }
//	inline void setSlot(int newSlot) { slot = newSlot; }
//	inline int getSlot() { return slot; }
//};

//Texture::Texture() {}
//void Texture::bind() {
//	glActiveTexture(GL_TEXTURE0 + slot);
//	image->bind();
//}
//void Texture::unbind() {
//	//glBindTexture(GL_TEXTURE_2D, 0);
//}