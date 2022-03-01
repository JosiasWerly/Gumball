#pragma once
#ifndef __texture
#define __texture
#include "AssetManager.hpp"
#include "GLUtils.hpp"
#include "Math.hpp"

#include <iostream>
#include "stb_image/stb_image.h"
using namespace std;



class Image :
	public Object {
	unsigned id = 0;
	int width = 0, height = 0;
	Color *imageBuffer = nullptr;
public:
	//code bloat? but purple is beauty! \o/
	~Image() {
		destroy();
	}
	Inline void create(int width, int height, Color *imageBuffer) {
		this->width = width;
		this->height = height;
		this->imageBuffer = imageBuffer;

		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, this->imageBuffer);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	Inline void destroy() {
		if (width > 0 || height > 0) {
			glDeleteTextures(1, &id);
			id = 0;
			width = 0;
			height = 0;
			delete[] imageBuffer;
			imageBuffer = nullptr;
		}
	}
	Inline void bind() {
		glBindTexture(GL_TEXTURE_2D, id);
	}
	Inline void unbind() {
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	Inline void uploadBuffer() {
		glBindTexture(GL_TEXTURE_2D, id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageBuffer);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	Inline void downloadBuffer() {
		throw 1;
	}
	Inline bool isValid() {
		return width > 0 || height > 0;
	}

	void setPixel(unsigned x, unsigned y, Color color) {
		const int p = ((y * height) + x) * 4;
		if (p > width * height)
			return;
		imageBuffer[p] = color;
	}
	Color getPixel(unsigned x, unsigned y) {
		const int p = ((y * height) + x) * 4;
		return imageBuffer[p];
	}
	Color *&getBuffer() { return imageBuffer; }
};

//TODO: texture should be a material parameter
class Texture {
protected:
	unsigned slot = 0;
public:
	Var<Image> image;
	
	Texture();
	void bind();
	void unbind();
	Inline void setSlot(int newSlot) { slot = newSlot; }
	Inline int getSlot() { return slot; }
};

class TextureFactory :
	public IAssetFactory {
public:
	TextureFactory() :
		IAssetFactory("TextureFactory") {
		this->extensions = { "png" };
	}
	virtual bool assemble(Asset &asset, Archive &ar);
	virtual bool disassemble(Asset &asset, Archive &ar) {
		return true;
	}
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