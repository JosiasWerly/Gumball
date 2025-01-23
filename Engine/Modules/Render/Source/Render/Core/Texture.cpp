#include "Texture.hpp"
#include "GLUtils.hpp"
#include "stbImage/stb_image.h"

Texture::Texture() {
}
Texture::~Texture() {
	destroy();
}
void Texture::upload() const {
	bind();
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, size.x, size.y, 0, format, type, buffer);
	unbind();
}
void Texture::bind() const {
	glBindTexture(GL_TEXTURE_2D, id);
}
void Texture::unbind() const {
	glBindTexture(GL_TEXTURE_2D, 0);
}
void Texture::create(Vector2i size, EInternalFormat internalFormat, EFormat format, EType type) {
	create(size, new Color[size.x * size.y], internalFormat, format, type);
}
void Texture::create(Vector2i size, Color *buffer, EInternalFormat internalFormat, EFormat format, EType type) {
	if (isValid()) {
		destroy();
	}

	//if (!buffer) {
	//	throw; //NO TEXTURE
	//	return;
	//}

	this->size = size;
	this->internalFormat = static_cast<unsigned>(internalFormat);
	this->type = static_cast<unsigned>(type);
	this->format = static_cast<unsigned>(format);
	this->buffer = buffer;

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexStorage2D(GL_TEXTURE_2D, 1, this->internalFormat, this->size.x, this->size.y);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (buffer) {
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, this->size.x, this->size.y, this->format, this->type, this->buffer);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
}
void Texture::destroy() {
	if (id > 0 && size.x > 0 || size.y > 0) { //TODO: change to isValid()?
		glDeleteTextures(1, &id);
		id = 0;
		size = { 0, 0 };
		delete[] buffer;
		buffer = nullptr;
	}
}
void Texture::setPixel(Vector2i pos, Color color) {
	const int p = ((pos.y * size.y) + pos.x) * 4; //COLOR Format?
	if (p > (size.x * size.y))
		return;
	buffer[p] = color;
}
Color Texture::getPixel(Vector2i pos) {
	const int p = ((pos.y * size.y) + pos.x) * 4;
	if (p > (size.x * size.y))
		return Color(0);
	return buffer[p];
}
void Texture::setBuffer(Color *newBuffer) {
	buffer = newBuffer;
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, size.x, size.y, format, type, buffer);
}

AssetFactory<Texture>::AssetFactory() {
	extensions = { "png" };
}
bool AssetFactory<Texture>::load(Archive &ar, Texture &val) {
	const string filePath = ar.getFilePath().getPath();
	ar.close();

	int ch = 0;
	int width = 0, height = 0;
	unsigned char *imageBuffer = nullptr;
	stbi_set_flip_vertically_on_load(true);
	if (imageBuffer = stbi_load(filePath.c_str(), &width, &height, &ch, 4)) {
		val.create({ width, height }, reinterpret_cast<Color *>(imageBuffer));
		return true;
	}
	return false;
}
bool AssetFactory<Texture>::save(Archive &ar, const Texture &val) {
	throw;
	return false;
}