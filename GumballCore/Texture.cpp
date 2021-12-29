#include "Texture.hpp"
#include <iostream>
#include "stb_image/stb_image.h"

bool TextureFactory::assemble(Asset &asset, Archive &ar) {
	const string filePath = ar.filePath();
	ar.close();//TODO: check this
	Image *img = new Image;

	int ch = 0;
	int width = 0, height = 0;
	unsigned char *memoryBuffer = nullptr;
	stbi_set_flip_vertically_on_load(true);
	if (memoryBuffer = stbi_load(filePath.c_str(), &width, &height, &ch, 4))
	{
		img->create(width, height, memoryBuffer);
		asset << img;
		return true;
	}
	else {
		delete img;
		return false;
	}	
}

	//const string filePath = ar.filePath();
	//ar.close();
	//
	//bind();
	//if (memoryBuffer) {
	//	stbi_image_free(memoryBuffer);
	//	memoryBuffer = nullptr;
	//}
	//
	//int ch = 0;
	//stbi_set_flip_vertically_on_load(true);
	//if (memoryBuffer = stbi_load(filePath.c_str(), &width, &height, &ch, 4)) {
	//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, memoryBuffer);
	//	glGenerateMipmap(GL_TEXTURE_2D);
	//}
	//unbind();
	//
	//
	//return memoryBuffer;