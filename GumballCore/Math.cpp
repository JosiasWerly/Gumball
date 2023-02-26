#include "Math.hpp"

void fRadToDegree(float &value) {
	value *= RadPI;
}
void fDegreeToRad(float &value) {
	value *= PIRad;
}

#pragma warning( disable : 4101 4244)




//	constexpr const Color Black(0x000000ff);
//	constexpr const Color White(0xffffffff);
//	constexpr Color Red = Color::Color(0xff0000ff);
//	constexpr const Color Green(0x00ff00ff);
//	constexpr const Color Blue(0x0000ffff);
//	constexpr const Color Yellow(0xffff00ff);
//	constexpr const Color Cyan(0x00ffffff);
//	constexpr const Color Magenta(0xff00ffff);
//	constexpr const Color Gray(0x808080ff);
//	constexpr const Color Brown(0xa52a2aff);
//	constexpr const Color Orange(0xffa500ff);
//	constexpr const Color Pink(0xffc0cbff);
//	constexpr const Color Purple(0x800080ff);


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