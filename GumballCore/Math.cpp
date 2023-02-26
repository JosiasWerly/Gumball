#include "Math.hpp"

void fRadToDegree(float &value) {
	value *= RadPI;
}
void fDegreeToRad(float &value) {
	value *= PIRad;
}

#pragma warning( disable : 4101 4244)

const Color Color::Black(0x000000ff);
const Color Color::White(0xffffffff);
const Color Color::Red(0xff0000ff);
const Color Color::Green(0x00ff00ff);
const Color Color::Blue(0x0000ffff);
const Color Color::Yellow(0xffff00ff);
const Color Color::Cyan(0x00ffffff);
const Color Color::Magenta(0xff00ffff);
const Color Color::Gray(0x808080ff);
const Color Color::Brown(0xa52a2aff);
const Color Color::Orange(0xffa500ff);
const Color Color::Pink(0xffc0cbff);
const Color Color::Purple(0x800080ff);

Color::operator glm::vec4() {
	auto out = glm::vec4(R, G, B, A);
	out /= 255.f;
	return out;
}


Vector3::operator Vector3i()	{ return Vector3i(x, y, z); }
Vector3::operator Vector2()		{ return Vector2(x, y); }
Vector3::operator Vector2i()	{ return Vector2i(x, y); }

Vector2::operator Vector3()		{ return Vector3(x, y, 0); }
Vector2::operator Vector3i()	{ return Vector3i(x, y, 0); }
Vector2::operator Vector2i()	{ return Vector2i(x, y); }

Vector3i::operator Vector3()	{ return Vector3(x, y, z); }
Vector3i::operator Vector2()	{ return Vector2(x, y); }
Vector3i::operator Vector2i()	{ return Vector2i(x, y); }

Vector2i::operator Vector3()	{ return Vector3(x, y, 0); }
Vector2i::operator Vector2()	{ return Vector2(x, y); }
Vector2i::operator Vector3i()	{ return Vector3i(x, y, 0); }
#pragma warning( default : 4101 4244)