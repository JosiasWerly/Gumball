#include "Math.hpp"

void fRadToDegree(float &value) {
	value *= RadPI;
}
void fDegreeToRad(float &value) {
	value *= PIRad;
}

#pragma warning( disable : 4101 4244)//loss of data warning...


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