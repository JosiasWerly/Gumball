#include "Drawable.hpp"
#include <iostream>
#include "stb_image/stb_image.h"

void Tbo::loadTexture(string path) {
	bind();
	if (imageBuffer) {
		stbi_image_free(imageBuffer);
		imageBuffer = nullptr;
	}	
	
	int ch = 0;
	stbi_set_flip_vertically_on_load(true);
	if (imageBuffer = stbi_load(path.c_str(), &width, &height, &ch, 4)) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageBuffer);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	unbind();
}