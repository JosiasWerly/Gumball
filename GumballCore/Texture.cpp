#include "Texture.hpp"
#include <iostream>
#include "stb_image/stb_image.h"

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