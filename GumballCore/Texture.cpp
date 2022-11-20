#include "Texture.hpp"

#include "Engine.hpp"
#include "stb_image/stb_image.h"
#include <iostream>


bool Image::archiveLoad(Archive &ar) {
	const string filePath = ar.filePath();
	ar.close();

	int ch = 0;
	int width = 0, height = 0;
	unsigned char *imageBuffer = nullptr;
	stbi_set_flip_vertically_on_load(true);
	if (imageBuffer = stbi_load(filePath.c_str(), &width, &height, &ch, 4)) {
		create(width, height, reinterpret_cast<Color *>(imageBuffer));
		return true;
	}
	return false;
}
bool Image::archiveSave(Archive &ar) {
	return false;
}

