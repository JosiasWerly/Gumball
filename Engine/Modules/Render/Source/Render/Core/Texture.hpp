#pragma once
#ifndef __texture
#define __texture
#include <iostream>
#include "Gumball/Archive.hpp"

class GMODULE Texture {
public:
	enum class EInternalFormat {
		rgba8 = 0x8058
	};
	enum class EFormat {
		rgba = 0x1908
	};
	enum class EType {
		ubyte = 0x1401
	};

private:
	Vector2i size;
	unsigned internalFormat = 0, type = 0, format = 0;
	Color *buffer = nullptr;
	unsigned id = 0;

public:
	Texture();
	~Texture();
	void upload() const;
	void bind() const;
	void unbind() const;
	void create(Vector2i size, EInternalFormat internalFormat = EInternalFormat::rgba8, EFormat format = EFormat::rgba, EType type = EType::ubyte);
	void create(Vector2i size, Color *buffer, EInternalFormat internalFormat = EInternalFormat::rgba8, EFormat format = EFormat::rgba, EType type = EType::ubyte);
	void destroy();
	void setBuffer(Color *newBuffer);
	void setPixel(Vector2i pos, Color color);
	Color getPixel(Vector2i pos);

	Inline Color *&getBuffer() { return buffer; }
	Inline bool isValid() const { return buffer; }
	Inline const unsigned &getId() { return id; }
};

template<>
class GMODULE WFileSerializer<Texture> : public TFileSerializer<Texture> {
public:
	bool load(const FilePath &fp, Texture &val);
	bool save(const FilePath &fp, Texture &val);
	bool supports(const FilePath &fp) const;
};
#endif // !__texture