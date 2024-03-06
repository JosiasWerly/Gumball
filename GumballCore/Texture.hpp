#pragma once
#ifndef __texture
#define __texture
#include "AssetSystem.hpp"
#include "GLUtils.hpp"
#include "GLBuffer.hpp"
#include "Math.hpp"

#include <iostream>
#include "stb_image/stb_image.h"
using namespace std;

//TODO: fix this texture filename
class GBCORE Image {
	Tbo textureBuffer;
public:

	~Image() {
		destroy();
	}
	void create(int width, int height, Color *imageBuffer) {
		textureBuffer.create(width, height, imageBuffer);
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
		textureBuffer.bind();
		textureBuffer.upload();
		textureBuffer.unbind();
	}
	Inline bool isValid() const {
		return textureBuffer.isValid();
	}
	Tbo &getTexture() { return textureBuffer; }
};

template<> class AssetFactory<Image> : public TAssetFactory<Image> {
public:
	AssetFactory();
	bool load(Archive &ar, Image &val);
	bool save(Archive &ar, const Image &val);
};

#endif // !__texture