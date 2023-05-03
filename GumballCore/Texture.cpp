#include "Texture.hpp"

#include "Engine.hpp"
#include "stb_image/stb_image.h"
#include <iostream>

AssetFactory<Image>::AssetFactory() {
	extensions = { "png" };
}
bool AssetFactory<Image>::load(Archive &ar, Image &val) {
	const string filePath = ar.filePath();
	ar.close();

	int ch = 0;
	int width = 0, height = 0;
	unsigned char *imageBuffer = nullptr;
	stbi_set_flip_vertically_on_load(true);
	if (imageBuffer = stbi_load(filePath.c_str(), &width, &height, &ch, 4)) {		
		val.create(width, height, reinterpret_cast<Color *>(imageBuffer));
		return true;
	}
	return false;
}
bool AssetFactory<Image>::save(Archive &ar, const Image &val) {
	throw;
	return false;
}