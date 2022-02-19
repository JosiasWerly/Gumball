#include "Texture.hpp"

#include "Engine.hpp"
#include "stb_image/stb_image.h"
#include <iostream>

bool TextureFactory::assemble(Asset &asset, Archive &ar) {
	const string filePath = ar.filePath();
	ar.close();//TODO: check this
	Image *img = new Image;

	int ch = 0;
	int width = 0, height = 0;
	unsigned char *imageBuffer = nullptr;
	stbi_set_flip_vertically_on_load(true);
	if (imageBuffer = stbi_load(filePath.c_str(), &width, &height, &ch, 4))	{
		img->create(width, height, reinterpret_cast<Color*>(imageBuffer));
		asset << img;
		return true;
	}
	else {
		delete img;
		return false;
	}
}


/*void setPixel(int x, int y, int color) {
	int p = ((y * height) + x) * 4;
	if (!memoryBuffer || p > width * height)
		return;
	memoryBuffer[p] = (color >> 24);
	memoryBuffer[p + 1] = (color >> 16);
	memoryBuffer[p + 2] = (color >> 8);
	memoryBuffer[p + 3] = color;
}*/
Texture::Texture() {}
bool Texture::setImage(string name) {
	AssetsSystem &assetSys = *Engine::instance()->getSystem<AssetsSystem>();

	Image *img = nullptr;
	if (assetSys(name, img))
		image = img;
	return image;
}
void Texture::bind() {
	glActiveTexture(GL_TEXTURE0 + slot);
	image->bind();
}
void Texture::unbind() {
	//glBindTexture(GL_TEXTURE_2D, 0);
}